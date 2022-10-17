#include "game.hpp"

#include "cxxopts.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    cxxopts::Options options("thegameanalyzer", "Play 'The Game' several times and give some stats.");

    options.add_options()("n,num-players", "Number of players", cxxopts::value<int>()->default_value("1"))                             //
        ("r,card-reach-distance", "How far to reach to play anther card (non-endgame)", cxxopts::value<int>()->default_value("1"))     //
        ("e,card-reach-distance-endgame", "How far to reach to play anther card (endgame)", cxxopts::value<int>()->default_value("1")) //
        ("s,seed", "Run the game once with random seed (0-10,000)", cxxopts::value<uint32_t>()->default_value("0"))                    //
        ("t,num-trials", "How many trials to play (1-10,000). If 1, print the game", cxxopts::value<int>()->default_value("1"))        //
        ("p,parallel", "Run trials in parallel")                                                                                       //
        ("h,help", "Print usage");

    const auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    // No bounds checking, just assert.
    const size_t seed = result["seed"].as<uint32_t>();
    const int num_players = result["num-players"].as<int>();
    const int card_reach_distance_normal = result["card-reach-distance"].as<int>();
    const int card_reach_distance_endgame = result["card-reach-distance-endgame"].as<int>();
    const int num_trials = result["num-trials"].as<int>();
    const bool do_parallel = result["parallel"].as<bool>();

    if (result.count("seed") || num_trials == 1)
    {
        int num_cards_remaining = TheGameAnalyzer::play_game(seed, num_players,
                                                             card_reach_distance_normal,
                                                             card_reach_distance_endgame,
                                                             TheGameAnalyzer::PrintGame::Yes);
        std::cout << "Cards remaining: " << num_cards_remaining << "\n";
    }
    else
    {
        const auto the_games_results = TheGameAnalyzer::play_games(num_players,
                                                                   card_reach_distance_normal,
                                                                   card_reach_distance_endgame,
                                                                   num_trials,
                                                                   do_parallel);

        std::cout << to_string(the_games_results) << "\n";
    }
    return 0;
}
