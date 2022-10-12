#pragma once

#include <cstdint>

namespace TheGameAnalyzer
{
    const int MIN_PLAYERS = 1;
    const int MAX_PLAYERS = 5;

    const int MIN_CARD_REACH_DISTANCE = 0;
    const int MAX_CARD_REACH_DISTANCE = 20;

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

} // namespace TheGameAnalyzer