# Output table of results for thegameanalyzer.
import subprocess
import json

NUM_TRIALS = 10_000


def print_table(num_players, title, results):
    # transpose the results table.
    results = [list(i) for i in zip(*results)]

    if num_players > 1:
        s = "s"
    else:
        s = ""
    print(f"## {num_players} player{s}: {title}")

    print(
        f"| reach distance (normal) | {' | '.join([str(i) for i in range(len(results[0]))])} |"
    )
    print(f"| reach distance (endgame) {'| ' * len(results[0])}|")
    for i, row in enumerate(results):
        print(f"| {i} | {' | '.join(row)} |")
    print()


def do_output_tables(
    num_players,
    card_reach_distance_normal_max,
    card_reach_distance_endgame_max,
):
    """Dump thegameanlyzer table to stdout.

    num_players: num players for this run.
    card_reach_distance_normal_max: 0 - card_reach_distance_normal for this run.
    card_reach_distance_endgame_min: m - card_reach_distance_endgame for this run.
    """

    excellent_results = []
    beat_the_game_results = []

    for r in range(card_reach_distance_normal_max + 1):

        excellent_results_row = [" "] * r
        beat_the_game_results_row = [" "] * r

        for e in range(r, card_reach_distance_endgame_max + 1):
            result = subprocess.run(
                [
                    "./thegameanalyzer",
                    "-p",
                    "-n",
                    str(num_players),
                    "-r",
                    str(r),
                    "-e",
                    str(e),
                    "-t",
                    str(NUM_TRIALS),
                ],
                stdout=subprocess.PIPE,
            )
            j = json.loads(result.stdout)

            excellent_results_row.append(str(j["excellent_percent"]))
            beat_the_game_results_row.append(str(j["beat_the_game_percent"]))

        excellent_results.append(excellent_results_row)
        beat_the_game_results.append(beat_the_game_results_row)

    print_table(
        num_players,
        "excellent game percentage (less than 10 cards remaining)",
        excellent_results,
    )
    print_table(
        num_players,
        "beat the game percentage (0 cards remaining)",
        beat_the_game_results,
    )


do_output_tables(1, 3, 4)
do_output_tables(2, 6, 13)
do_output_tables(3, 6, 13)
do_output_tables(4, 6, 13)
do_output_tables(5, 6, 13)