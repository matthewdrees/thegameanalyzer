#include "games.hpp"

#include "game.hpp"
#include "turn.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

using namespace TheGameAnalyzer;

namespace TheGameAnalyzer
{
    void draw_cards(std::vector<Card> &deck, Hand &hand, HandMask hand_mask);
}

int test_draw_cards()
{
    struct TestCase
    {
        std::vector<Card> deck;
        Hand hand;
        HandMask hand_mask;
        std::vector<Card> exp_deck;
        Hand exp_hand;
    };
    const TestCase test_cases[] = {
        {{}, {26, 29, 93}, 0x4, {}, {26, 29}},
        {{7}, {26, 29, 93}, 0x4, {}, {7, 26, 29}},
        {{7}, {26, 29, 93}, 0x6, {}, {7, 26}},
        {{7}, {26, 29, 93}, 0x3, {}, {7, 93}},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        std::vector<Card> act_deck = tc.deck;
        Hand act_hand = tc.hand;
        draw_cards(act_deck, act_hand, tc.hand_mask);
        if (tc.exp_deck != act_deck || tc.exp_hand != act_hand)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(deck: " << to_string(tc.deck)
                      << ", hand: " << to_string(tc.hand)
                      << ", hand_mask: " << std::hex << tc.hand_mask << std::dec
                      << ", exp_deck: " << to_string(tc.exp_deck)
                      << ", act_deck: " << to_string(act_deck)
                      << ", exp_hand: " << to_string(tc.exp_hand)
                      << ", act_hand: " << to_string(act_hand) << '\n';
        }
    }
    return num_fails;
}

// This is good enough.
bool close_enough(const TheGamesResults &tgr1, const TheGamesResults &tgr2, double delta)
{
    return abs(tgr1.excellent_percent - tgr2.excellent_percent) <= delta &&
           abs(tgr1.beat_the_game_percent - tgr2.beat_the_game_percent) <= delta &&
           abs(tgr1.cards_left_average - tgr2.cards_left_average) <= delta &&
           abs(tgr1.cards_left_stddev - tgr2.cards_left_stddev) <= delta;
}

int test_calculate_games_stats()
{
    struct TestCase
    {
        std::vector<int> num_cards_played;
        TheGamesResults exp;
    };
    const TestCase test_cases[] = {
        {{0}, {100.0, 100.0, 0.0, 0.0}},
        {{1}, {100.0, 0.0, 1.0, 0.0}},
        {{9}, {100.0, 0.0, 9.0, 0.0}},
        {{10}, {0.0, 0.0, 10.0, 0.0}},
        {{10, 0}, {50.0, 50.0, 5.0, 5.0}},
        {{11, 4, 0, 7}, {75.0, 25.0, 5.5, 4.031128874}},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const auto act = calculate_games_stats(tc.num_cards_played);
        if (!close_enough(tc.exp, act, 0.0001))
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(num_cards_played: " << tc.num_cards_played.size() << ")" << '\n'
                      << ", exp: " << to_string(tc.exp) << '\n'
                      << ", act: " << to_string(act) << '\n';
        }
    }
    return num_fails;
}

int main()
{
    const int num_fails = test_draw_cards() +
                          test_calculate_games_stats();

    return num_fails != 0;
}