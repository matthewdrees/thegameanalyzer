#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace TheGameAnalyzer
{
    const int MIN_PLAYERS = 1;
    const int MAX_PLAYERS = 5;

    const int MIN_CARD_REACH_DISTANCE = 0;
    const int MAX_CARD_REACH_DISTANCE = 20;

    const int MIN_TRIALS = 1;
    const int MAX_TRIALS = 10'000;

    enum class PrintGame
    {
        No,
        Yes
    };

    // Play the game.
    //
    // \param seed Seed for random deck shuffle.
    // \param num_players Number of players in the game (1-5).
    // \param card_reach_distance_normal How much to reach for playing another card (before the endgame).
    // \param card_reach_distance_endgame How much to reach for playing another card during the endgame.
    // \param do_print_game If true print the game to stdout.
    // \return number of cards remaining.
    int play_game(uint32_t seed, int num_players, int card_reach_distance_normal,
                  int card_reach_distance_endgame, PrintGame print_game);

    struct TheGamesResults
    {
        double excellent_percent = 0.0f;     // Percentage of games with an "excellent" finish.
        double beat_the_game_percent = 0.0f; // Percentage of games that beat the game.
        double cards_left_average = 0.0f;    // Average number of cards remaining.
        double cards_left_stddev = 0.0f;     // Standard deviation of cards remaining.
    };

    std::string to_string(const TheGamesResults &the_games_results);

    // Calculate results from several games.
    TheGamesResults calculate_games_stats(const std::vector<int> &num_cards_played);

    // Play several trials of the game.
    //
    // \param num_players Number of players in the game (1-5).
    // \param card_reach_distance How much to reach for playing another card (before the endgame).
    // \param card_reach_distance_endgame How much to reach for playing another card during the endgame.
    // \param num_trials Number of trials to run (1-10,000). Note if num_trials is 1,
    //                    print_game is set to true, otherwise false.
    // \param do_parallel If true run the trials in parallel.
    // \return Statistics for playing several trials of the game.
    TheGamesResults play_games(int num_players, int card_reach_distance, int card_reach_distance_endgame,
                               int num_trials, bool do_parallel);

} // namespace TheGameAnalyzer