#include "game.hpp"

#include "turn.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

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
    void draw_cards(std::vector<Card> &deck, Hand &hand, HandMask hand_mask)
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
        std::sort(hand.begin(), hand.end());
    }

    size_t get_strongest_starting_hands_index(const Piles &piles, const std::vector<Hand> &hands,
                                              int min_cards_for_turn, int card_reach_distance)
    {
        std::vector<Turn> turns(hands.size());
        std::transform(hands.begin(), hands.end(), turns.begin(), [=, piles = std::cref(piles)](const auto &h)
                       { return find_best_turn(piles, h, min_cards_for_turn, card_reach_distance); });
        const TurnCompare turn_compare{min_cards_for_turn, card_reach_distance};
        const auto max_turn_it = std::max_element(turns.begin(), turns.end(), turn_compare);
        return static_cast<size_t>(max_turn_it - turns.begin());
    }

    int play_game(uint32_t seed, int num_players, int card_reach_distance_normal,
                  int card_reach_distance_endgame, PrintGame print_game)
    {
        assert(num_players >= MIN_PLAYERS);
        assert(num_players <= MAX_PLAYERS);
        assert(card_reach_distance_normal >= MIN_CARD_REACH_DISTANCE);
        assert(card_reach_distance_normal <= MAX_CARD_REACH_DISTANCE);
        assert(card_reach_distance_endgame >= MIN_CARD_REACH_DISTANCE);
        assert(card_reach_distance_endgame <= MAX_CARD_REACH_DISTANCE);

        Piles piles = {1, 1, 100, 100};
        std::vector<Hand> hands(static_cast<size_t>(num_players));

        // Generate the deck [2 - 99] and shuffle.
        const int NUM_CARDS_IN_DECK = 98;
        int num_cards_in_game = NUM_CARDS_IN_DECK;
        std::vector<Card> deck(static_cast<size_t>(NUM_CARDS_IN_DECK));
        std::iota(deck.begin(), deck.end(), 2);
        std::mt19937 gen32(seed);
        std::shuffle(deck.begin(), deck.end(), gen32);

        // Deal the hands.
        const auto num_cards_per_hand = calc_num_cards_per_hand(num_players);
        for (auto &hand : hands)
        {
            hand.resize(num_cards_per_hand);
            std::copy(deck.end() - num_cards_per_hand, deck.end(), hand.begin());
            std::sort(hand.begin(), hand.end());
            deck.erase(deck.end() - num_cards_per_hand, deck.end());
        }

        if (print_game == PrintGame::Yes)
        {
            std::cout << "seed: " << seed << ", deck: " << to_string(deck) << "\n";
        }

        const int STARTING_MIN_CARDS_PER_TURN = 2;
        // Get the strongest starting hand
        auto hands_index = get_strongest_starting_hands_index(piles, hands, STARTING_MIN_CARDS_PER_TURN, card_reach_distance_normal);

        // Play the game.
        while (num_cards_in_game > 0)
        {
            auto &hand = hands[hands_index];
            if (!hand.empty())
            {
                const int min_cards_for_turn = deck.empty() ? 1 : STARTING_MIN_CARDS_PER_TURN;
                const int card_reach_distance = deck.empty() ? card_reach_distance_endgame : card_reach_distance_normal;
                const auto turn = find_best_turn(piles, hand, min_cards_for_turn, card_reach_distance);
                if (print_game == PrintGame::Yes)
                {
                    std::cout << to_string(piles) << ", hand: " << hands_index << ", "
                              << to_string(hand) << ", 0x" << std::hex << turn.hand_mask << std::dec
                              << "\n";
                }
                const auto num_cards_played = get_num_cards_in_hand_mask(turn.hand_mask);
                num_cards_in_game -= num_cards_played;
                if (num_cards_played < min_cards_for_turn)
                {
                    break;
                }
                piles = turn.piles;
                draw_cards(deck, hand, turn.hand_mask);
            }
            ++hands_index;
            if (hands_index == hands.size())
            {
                hands_index = 0;
            }
        }
        return num_cards_in_game;
    }
} // namespace TheGameAnalyzer