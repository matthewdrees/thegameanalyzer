#include "games.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

using namespace TheGameAnalyzer;

// This is good enough.
bool close_enough(const TheGameResults &tgr1, TheGameResults &tgr2, double delta)
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
        TheGamesResults expected;
    };

    TestCase test_cases[] = {
        {{1}, {100.0, 100.0, 1.0, 0.0}},
        {{9}, {0.0, 100.0, 9.0, 0.0}},
        {{10}, {0.0, 0.0, 10.0, 0.0}},
        {{10, 0}, {10.0, 50.0, 5.0, 5.0}},
        {{11, 4, 0, 7}, {25.0, 75.0, 5.5, 4.031128874}},
    };

    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const auto actual = calculate_game_stats(tc.num_cards_played);
        if (!close_enough(tc.expected, actual, 0.0001))
        {
            ++num_fails;
            std::cerr << "FAIL, " << __FUNCTION__
                      << "(hand: " << to_string(tc.hand) << ")"
                      << ", exp: " << to_string(tc.exp)
                      << ", act: " << to_string(act) << '\n';
        }
    }
    return num_fails;
}

int main()
{
    const int num_fails = test_calculate_games_stats();

    return num_fails != 0;
}