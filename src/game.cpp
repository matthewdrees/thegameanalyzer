#include "game.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

static size_t calc_num_cards_per_hand(size_t num_players)
{
    switch (num_players)
    {
    case 1:
        return 8;
    case 2:
        return 7;
    case 3:
    case 4:
    case 5:
        return 6;
    }
    assert(false);
}

namespace TheGameAnalyzer
{
    void draw_cards(std::vector<Card> &deck, Hand &hand, unsigned hand_mask)
    {
        for (size_t i = 0; i < hand.size(); ++i)
        {
            const unsigned card_mask = 1 << i;
            if ((card_mask & hand_mask) != 0)
            {
                hand[i] = 0;
            }
        }
        auto hand_it = std::partition(hand.begin(), hand.end(), [](const Card c)
                                      { return c > 0; });
        auto num_cards_to_replace = std::min(static_cast<size_t>(hand.end() - hand_it), deck.size());
        auto deck_it = deck.end() - num_cards_to_replace;
        hand.erase(std::copy(deck_it, deck.end(), hand_it), hand.end());
        deck.erase(deck_it, deck.end());
    }

    enum class PrintGame
    {
        No,
        Yes
    };

    struct GameSettings
    {
        size_t num_players{1};
        int starting_min_num_cards{2};
        int extra_card_distance{1};
        int extra_card_distance_end_game{1};
        PrintGame print_game{PrintGame::No};
    };
    struct GameState
    {
        GameState(unsigned seed, size_t num_players);

        Piles piles = {1, 1, 100, 100};
        std::vector<Hand> hands;
        std::vector<Card> deck;
        int num_cards_in_game{98};
    };

    size_t get_strongest_starting_hands_index(const TheGameAnalyzer::GameState &gs, int extra_card_distance)
    {
        std::vector<Turn> turns(gs.hands.size());
        std::transform(gs.hands.begin(), gs.hands.end(), turns.begin(), [&](const auto &h)
                       { return find_best_turn(gs.piles, h, 2, game_settings.extra_card_distance); });
        const auto max_turn_it = std::max_element(turns.begin(), turns.end());
        return static_cast<size_t>(max_turn_it - turns.begin());
    }

    GameState::GameState(unsigned seed, size_t num_players) : hands(num_players),
                                                              deck(98)
    {
        // Generate and shuffle the deck.
        std::iota(deck.begin(), deck.end(), 2);
        std::mt19937 gen32(seed);
        std::shuffle(deck.begin(), deck.end(), gen32);

        const auto num_cards_per_hand = calc_num_cards_per_hand(num_players);

        // Deal the hands.
        for (auto &hand : hands)
        {
            draw_cards(deck, hand, num_cards_per_hand);
        }
    }

    GameState play_game(unsigned seed, const GameSettings &game_settings)
    {
        GameState gs(seed, game_settings.num_players);
        auto hands_index = get_strongest_starting_hands_index(gs, game_settings.extra_card_distance);
        while (gs.num_cards_in_game > 0)
        {
            auto &hand = gs.hands[hands_index];
            if (hand.empty())
            {
                continue;
            }
            const int min_num_cards = gs.deck.empty() ? 1 : game_settings.starting_min_num_cards;
            const int extra_card_distance = gs.deck.empty() ? game_settings.extra_card_distance_end_game : game_settings.extra_card_distance;
            const auto turn = find_best_turn(gs.piles, hand, min_num_cards, extra_card_distance);
            if (game_settings.print_game == PrintGame::Yes)
            {
            }
            gs.num_cards_in_game -= turn.num_cards_played;
            if (turn.num_cards_needed > 0)
            {
                break;
            }
            draw_cards(gs.deck, hand, turn.hand_mask);

            ++hands_index;
            if (hands_index == gs.hands.size())
            {
                hands_index = 0;
            }
        }

        return gs;
    }

    int play_game(uint32_t seed, int num_players, int card_reach_distance,
                  int card_reach_distance_endgame, bool do_print_game)
    {
    }
} // namespace TheGameAnalyzer