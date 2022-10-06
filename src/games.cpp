#include "games.hpp"

#include <cmath>

namespace TheGameAnalyzer
{
    TheGamesResults calculate_games_stats(const std::vector<int> &)
    {
        // const double average = static_cast<double>(std::accumulate(...)) / card_remaining_vec.size();

        // std::count_if
        // double stddev =
        // std::sqrt()
        // std::accumulate
        // for i in card_remaining_vec:
        //     running_sum_square += std::pow(static_cast<double>(i)-average, 2)
        // divided by card_remaining_vec.size()
    }

    TheGamesResults play_games(int num_players, int card_reach_distance, int card_reach_distance_endgame,
                               int num_trials, bool do_parallel)
    {
        // Use std::vector<size_t> for seeds.
        // std::vector<int> for play_game return
        // std::transform with std::seq or std::par_unseq
        // do_print_game is true if num_trials == 1
    }
}