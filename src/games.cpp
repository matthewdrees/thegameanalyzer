#include "games.hpp"

#include "game.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <execution>
#include <numeric>
#include <sstream>
#include <vector>

namespace TheGameAnalyzer
{

    std::string to_string(const TheGamesResults &tgr)
    {
        std::ostringstream oss;
        oss << "{ excellent_percent: " << tgr.excellent_percent
            << ", beat_the_game_percent: " << tgr.beat_the_game_percent
            << ", cards_left_average: " << tgr.cards_left_average
            << ", cards_left_stddev: " << tgr.cards_left_stddev
            << "}";
        return oss.str();
    }

    TheGamesResults calculate_games_stats(const std::vector<int> &num_cards_played)
    {
        assert(num_cards_played.size() > 0);
        TheGamesResults results;
        const double num_games = static_cast<double>(num_cards_played.size());
        results.excellent_percent = std::count_if(num_cards_played.begin(), num_cards_played.end(), [](auto i)
                                                  { return i < 10; }) /
                                    num_games * 100.0;
        results.beat_the_game_percent = std::count_if(num_cards_played.begin(), num_cards_played.end(), [](auto i)
                                                      { return i == 0; }) /
                                        num_games * 100.0;
        results.cards_left_average = std::accumulate(num_cards_played.begin(), num_cards_played.end(), 0) / num_games;
        results.cards_left_stddev = std::sqrt(std::accumulate(num_cards_played.begin(), num_cards_played.end(), 0.0, [&](double d, int i)
                                                              { return d + std::pow(static_cast<double>(i) - results.cards_left_average, 2); }) /
                                              num_games);
        return results;
    }

    TheGamesResults play_games(int num_players, int card_reach_distance_normal, int card_reach_distance_endgame,
                               int num_trials, bool do_parallel)
    {
        assert(num_trials >= MIN_TRIALS);
        assert(num_trials <= MAX_TRIALS);
        std::vector<size_t> seeds(static_cast<size_t>(num_trials));
        std::iota(seeds.begin(), seeds.end(), 0);
        std::vector<int> num_cards_played(static_cast<size_t>(num_trials));
        const auto print_game = num_trials == 1 ? PrintGame::Yes : PrintGame::No;
        // Blah. Is this any better? https://stackoverflow.com/questions/52975114/different-execution-policies-at-runtime
        if (do_parallel)
        {
            std::transform(std::execution::par, seeds.begin(), seeds.end(), num_cards_played.begin(), [=](auto seed)
                           { return play_game(seed, num_players, card_reach_distance_normal, card_reach_distance_endgame, print_game); });
        }
        else
        {
            std::transform(std::execution::seq, seeds.begin(), seeds.end(), num_cards_played.begin(), [=](auto seed)
                           { return play_game(seed, num_players, card_reach_distance_normal, card_reach_distance_endgame, print_game); });
        }
        return calculate_games_stats(num_cards_played);
    }
} // namespace TheGameAnalyzer