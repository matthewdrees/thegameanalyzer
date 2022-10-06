#include <cxxopts.h>

#include "games.hpp"

int main(int argc, char *argv[])
{
    cxxopts::Options options("thegameanalyzer", "Play 'The Game' several times and give some stats.");

    options.add_options()("n,num-players", "Number of players", cxxopts::value<int>()->default_value("1"))                             //
        ("r,card-reach-distance", "How far to reach to play anther card (non-endgame)", cxxopts::value<int>()->default_value("1"))     //
        ("e,card-reach-distance-endgame", "How far to reach to play anther card (endgame)", cxxopts::value<int>()->default_value("1")) //
        ("t,num-trials", "How many trials to play (1-10,000). If 1, print the game", cxxopts::value<int>()->default_value("1"))        //
        ("p,parallel", "Run trials in parallel")                                                                                       //
        ("h,help", "Print usage");

    const auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }
    // TODO
    // No bounds checking, just assert.
    // const auto the_games_results = TheGameAnalyzer::play_games(num_players, int card_reach_distance, int card_reach_distance_endgame,
    //                            int num_trials, bool do_parallel);
    // if num_trials is > 1, print the_game_results

    return 0;
}
