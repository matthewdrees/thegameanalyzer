# thegameanalyzer

Analyze outcomes for the [The Game (card game)](https://boardgamegeek.com/boardgame/173090/game).

With a basic strategy, how likely is that you have an excellent result (< 10 cards remaining) or beat the game (0 cards remaining)?

## Results

(TODO: values are placeholders and not correct!!!)

| # players | excellent result % | beat the game % | avg cards remaining | stddev | # reach | # reach (endgame) |
|---|---|---|---|---|---|--|
| 1 | 30.23% | 2.11 % | 5.3 | 1.2 | 4 | 6 |
| 2 | 30.23% | 2.11 % | 5.3 | 1.2 | 4 | 6 |
| 3 | 30.23% | 2.11 % | 5.3 | 1.2 | 4 | 6 |
| 4 | 30.23% | 2.11 % | 5.3 | 1.2 | 4 | 6 |
| 5 | 30.23% | 2.11 % | 5.3 | 1.2 | 4 | 6 |

(See below for more details about each run.)

## Basic strategy assumptions

* Play the minimum cards that result with the lowest change. (Except see the notes about _reaching_.)
* Tiebreaker goes to the bigger pile (e.g. if deciding between two _up_ piles, play on the higher number to leave room on the lower number pile).
* No communication between players. Per the rules, players can communicate by saying things like "Don't place on the last pile", or "Don't make a big jump on this pile", but I didn't model this.
* No memory of what cards have been played.
* Reaching: If you play the minimum number of cards and have an extra card that is 1 more, it seems wise to play it. But how high do you reach to play the next card? Don't know, so modelled it with a "# reach". E.g. 0 means don't reach at all, 3 means reach for cards that are up to 3 higher, etc.
* Reaching in the endgame: When the draw pile is empty players only have to play 1 card. I call this phase the endgame. It seems like you should be able to reach further in the endgame, so I modelled it with "# reach (endgame)".
* Reaching for chained 10-groups. Let's say you have 10, 15, 20, 25 and an "up" pile showing 23 and no other piles in play. Note the 2x 10-groups: [10, 20], and [15, 25]. The play of 25 and 15 will happen because it plays the minimum number of cards and lowers the "up" pile from 23 to 15. Should you play the following 20 and 10? It would leave the pile lower, but might be a wasteful play of a precious 10-group. To decide this I used the "# reach". So in this case if "# reach" was >= 5 it would play the 20 and 10 (reaching from 15 to 20).
* Reaching for the bottom edge of a 10-group. Let's say you have 15, 17, 20, 30 with an "up" pile showing 10, no other piles in play, and a "# reach" of 4. The play of 15 and 17 handles the minimum number of cards to play. The 20 is 3 away, which is within our reach threshold of 4. Is playing this [20, 30] 10-group worth it? (Obviously if we play the 20 we might as well play the 30 first.) To decide this the model ignores the near edge of the group for reaching. So in this case the model does not reach for this group. However, if the 21, and 26 were also in the hand, the chain of those numbers _would_ put the far edge of the 10-group in play and the pile would play like this: 15, 17, 21, 26, 30, 20.

## Notes about each run

* Each run plays TODO games, which was enough to settle the "excellent result %".
* Each run has the same decks. (I used a shuffle algorithm with the same seeds.)

## Analysis runs for 1 player

| # reach | # reach (endgame) | avg cards remaining | stddev | excellent result % | beat the game % |
|---|---|---|---|---|---|
| 0 | 0 | 11.0 | 3.5 | 30.23% | 2.11% |
| 1 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 1 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 1 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |
| 2 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 2 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 2 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |
| 3 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 3 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 3 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |
| 4 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 4 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 4 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |
| 5 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 5 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 5 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |
| 6 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 6 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 6 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |
| 7 | 4 | 11.0 | 3.5 | 30.23% | 2.11% |
| 7 | 5 | 11.0 | 3.5 | 30.23% | 2.11% |
| 7 | 6 | 11.0 | 3.5 | 30.23% | 2.11% |

## Analysis runs for 2 players

## Analysis runs for 3 players

## Analysis runs for 4 players

## Analysis runs for 5 players
