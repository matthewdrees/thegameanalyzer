# thegameanalyzer

We are having a lot of fun playing [The Game (card game)](https://boardgamegeek.com/boardgame/173090/game).

Three questions arose:
1. What is the ideal _reach distance_ for playing extra cards on a turn?
2. With this optimal basic strategy, how likely is that you have an excellent result (less than 10 cards remaining) or beat the game (0 cards remaining)?
3. I once played 6 cards on a turn for a combined pile change of 30 in a 4 player game. This was not well received at the time, and remains criticized to this day. Was it a dumb move? (I can answer this now, that style of play reduces the chances of an excellent result and/or beating the game by ~2%, so "yes".)

## What is the ideal reach distance to play extra cards?

For 1 player, reach up to 1 above pile cards in all phases of the game. This makes intuitive sense and bore out with the data.

For 2-5 players, reach up to ~3 above piles cards while there are cards in the deck. When the deck is empty reach for ~6-11. There are small differences depending on the number of players or if you're optimizing for _less than ten_ cards or _zero_ cards. See tables below for more details.

## How well does a good basic strategy work?

| # players | excellent result % (less than 10 cards remaining) | beat the game % (0 cards remaining) |
|---|---|---|
| 1 | 45% | 12% |
| 2 | 75% | 18% |
| 3 | 59% | 8% |
| 4 | 73% | 11% |
| 5 | 76% | 12% |

See tables below for more details.

## Basic strategy assumptions

* No communication between players except for determining the strongest starting hand. Per the rules, players can communicate by saying things like "Don't place on the last pile", or "Don't make a big jump on this pile", but I didn't model this.
* No memory of what cards have been played.
* Play the minimum cards that result with the lowest change, then reach for a _reach distance_ for other cards, if any.
* Tiebreakers for playing cards during a turn:
  1. Play the minimum number of cards.
  2. Prefer the smaller total delta for minimum number of cards.
  3. Don't reach for the near card of a 10-group (which wastes the group. This improved excellent results by about 1% across the board). It's fine to reach for the far card of a 10-group.
  4. Prefer the turn that used more cards.
  5. Play on the more-advanced piles rather than the less-advanced piles, to keep them available for other playing options.
* Playing extra cards: If you play the minimum number of cards and have an extra card that is 1 more, it seems wise to play it. But how high do you reach to play the next card? Don't know, so modelled it with a _reach distance_. E.g. 0 means don't play any extra cards, 3 means play extra for cards that are up to 3 higher, etc.
* Playing extra cards during the endgame: If the deck is empty, use the _endgame_ reach distance, otherwise _normal_. (It seems intuitive to reach more to play extra cards during the endgame, right?)

## Notes about each run

* Each run plays 10, 000 games.
* Each run has the same decks. (I used the same shuffle algorithm with the same random seeds.)

## 1 player: excellent game percentage (less than 10 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 |
|---|---|---|---|---|
| reach distance (endgame ↓) | | | | |
| 0 | 44.53 |   |   |   |
| 1 | 44.53 | 44.86 |   |   |
| 2 | 44.53 | 44.86 | 44 |   |
| 3 | 44.53 | 44.86 | 44 | 42.45 |
| 4 | 44.53 | 44.86 | 44 | 42.45 |

## 1 player: beat the game percentage (0 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 |
|---|---|---|---|---|
| reach distance (endgame ↓) | | | | |
| 0 | 11.77 |   |   |   |
| 1 | 11.77 | 11.41 |   |   |
| 2 | 11.77 | 11.41 | 10.87 |   |
| 3 | 11.77 | 11.41 | 10.87 | 9.6 |
| 4 | 11.77 | 11.41 | 10.87 | 9.6 |

## 2 players: excellent game percentage (less than 10 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 65.78 |   |   |   |   |   |   |
| 1 | 66.4 | 70.3 |   |   |   |   |   |
| 2 | 66.96 | 70.75 | 73.1 |   |   |   |   |
| 3 | 67.44 | 71.11 | 73.4 | 73.55 |   |   |   |
| 4 | 67.76 | 71.38 | 73.71 | 73.77 | 73.98 |   |   |
| 5 | 68.02 | 71.66 | 73.91 | 74 | 74.29 | 72.02 |   |
| 6 | 68.21 | 71.94 | 74.07 | 74.18 | 74.55 | 72.19 | 69.66 |
| 7 | 68.35 | 72.02 | 74.19 | 74.2 | 74.77 | 72.32 | 69.74 |
| 8 | 68.5 | 72.12 | 74.34 | 74.37 | 74.82 | 72.56 | 69.9 |
| 9 | 68.56 | 72.24 | 74.36 | 74.55 | 74.93 | 72.7 | 69.95 |
| 10 | 68.54 | 72.27 | 74.39 | 74.56 | 74.93 | 72.78 | 70.05 |
| 11 | 68.67 | 72.29 | 74.47 | 74.68 | 74.99 | 72.83 | 70.08 |
| 12 | 68.69 | 72.32 | 74.44 | 74.67 | 74.94 | 72.97 | 70.12 |
| 13 | 68.69 | 72.32 | 74.52 | 74.73 | 74.99 | 72.98 | 70.12 |

## 2 players: beat the game percentage (0 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 12.28 |   |   |   |   |   |   |
| 1 | 13.62 | 15.07 |   |   |   |   |   |
| 2 | 14.67 | 16.07 | 16.83 |   |   |   |   |
| 3 | 15.51 | 16.96 | 17.72 | 16.78 |   |   |   |
| 4 | 15.96 | 17.38 | 18.22 | 17.37 | 16.29 |   |   |
| 5 | 16.1 | 17.89 | 18.47 | 17.71 | 16.5 | 14.4 |   |
| 6 | 16.29 | 17.96 | 18.8 | 17.93 | 16.52 | 14.69 | 12.88 |
| 7 | 16.36 | 18.17 | 18.81 | 18.13 | 16.81 | 14.64 | 13.02 |
| 8 | 16.26 | 18.15 | 18.72 | 18.36 | 17.06 | 14.76 | 13.09 |
| 9 | 16.31 | 18.06 | 18.43 | 18.54 | 16.85 | 14.82 | 13.06 |
| 10 | 16.37 | 18.03 | 18.61 | 18.53 | 16.86 | 14.99 | 13.13 |
| 11 | 16.04 | 17.86 | 18.47 | 18.05 | 16.57 | 14.91 | 12.91 |
| 12 | 15.59 | 17.26 | 18.26 | 17.72 | 16.31 | 14.8 | 12.57 |
| 13 | 15.52 | 17.08 | 17.79 | 17.51 | 16.05 | 14.6 | 12.5 |

## 3 players: excellent game % (less than 10 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 47.49|||||| |
| 1 | 48.6|51.84||||| |
| 2 | 49.53|52.66|55.06|||| |
| 3 | 50.2|53.55|55.79|57.17||| |
| 4 | 50.71|54.03|56.29|57.84|57.05|| |
| 5 | 51|54.44|56.83|58.28|57.48|55.79| |
| 6 | 51.45|55.04|57.28|58.67|57.99|56.31|52.71 |
| 7 | 51.61|55.19|57.62|58.94|58.16|56.81|53.11 |
| 8 | 51.87|55.24|57.82|59.04|58.51|56.95|53.55 |
| 9 | 51.87|55.2|57.94|59.15|58.69|56.93|53.6 |
| 10 | 51.89|55.08|57.94|59.14|58.8|56.89|53.51 |
| 11 | 52.04|55.05|57.89|59.1|58.7|56.93|53.39 |
| 12 | 51.74|55.07|57.72|59.23|58.57|56.86|53.19 |
| 13 | 51.64|54.86|57.49|59.05|58.4|56.74|52.9 |

## 3 players: beat the game percentage (0 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 4.22|||||| |
| 1 | 4.99|5.36||||| |
| 2 | 5.43|5.93|6.53|||| |
| 3 | 6.06|6.47|7|7||| |
| 4 | 6.17|6.91|7.41|6.92|6.84|| |
| 5 | 6.38|7.2|7.62|6.72|6.9|6.44| |
| 6 | 6.26|7.27|7.64|7.14|7.21|6.37|5.03 |
| 7 | 6.1|7.24|7.82|7.35|7.18|6.54|5.01 |
| 8 | 6.27|7.35|7.73|7.35|7.19|6.39|5.01 |
| 9 | 6.21|7.18|7.56|7.3|7.07|6.35|5.09 |
| 10 | 6.09|7.17|7.49|7.34|7.05|6.23|5.12 |
| 11 | 5.78|6.95|7.35|7.23|6.82|5.98|5.06 |
| 12 | 5.58|6.69|7.02|6.91|6.6|5.89|4.98 |
| 13 | 5.31|6.49|6.83|6.79|6.51|5.57|4.79 |

## 4 players: excellent game percentage (less than 10 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 61.79 |   |   |   |   |   |   |
| 1 | 64.41 | 66.13 |   |   |   |   |   |
| 2 | 65.86 | 67.83 | 69.11 |   |   |   |   |
| 3 | 66.91 | 68.93 | 70.55 | 71.31 |   |   |   |
| 4 | 67.81 | 69.64 | 71.62 | 71.94 | 71.53 |   |   |
| 5 | 68.19 | 70.15 | 72 | 72.51 | 71.99 | 70.29 |   |
| 6 | 68.58 | 70.36 | 71.92 | 73.23 | 72.46 | 70.88 | 68.03 |
| 7 | 68.78 | 71 | 72 | 73.54 | 72.46 | 71.18 | 68.2 |
| 8 | 68.69 | 70.74 | 71.88 | 73.26 | 72.37 | 71.65 | 68 |
| 9 | 68.23 | 70.69 | 71.91 | 73.28 | 72.4 | 71.33 | 67.8 |
| 10 | 67.94 | 70.5 | 71.93 | 73.34 | 72.14 | 71.29 | 67.6 |
| 11 | 67.41 | 69.96 | 71.43 | 72.9 | 71.99 | 70.65 | 67.09 |
| 12 | 66.84 | 69.36 | 71.1 | 72.45 | 71.44 | 70.05 | 66.67 |
| 13 | 66.63 | 68.93 | 70.53 | 72.36 | 71.1 | 69.53 | 66.32 |

## 4 players: beat the game percentage (0 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 6.83 |   |   |   |   |   |   |
| 1 | 7.92 | 8.45 |   |   |   |   |   |
| 2 | 9.01 | 9.78 | 10 |   |   |   |   |
| 3 | 9.65 | 10.32 | 10.76 | 10.7 |   |   |   |
| 4 | 9.72 | 11.03 | 10.86 | 10.8 | 9.73 |   |   |
| 5 | 9.95 | 11.19 | 10.87 | 11.24 | 9.81 | 8.95 |   |
| 6 | 10 | 10.8 | 11.05 | 11.07 | 10 | 9.11 | 7.35 |
| 7 | 10.06 | 10.71 | 10.99 | 11.08 | 9.89 | 9.22 | 7.15 |
| 8 | 9.97 | 10.4 | 10.78 | 10.98 | 9.65 | 9.04 | 7.01 |
| 9 | 10 | 10.22 | 10.55 | 10.44 | 9.38 | 8.86 | 6.81 |
| 10 | 9.88 | 10.17 | 10.62 | 10.1 | 9.23 | 8.74 | 6.81 |
| 11 | 8.98 | 9.52 | 9.96 | 9.68 | 8.75 | 8.51 | 6.8 |
| 12 | 8.63 | 9.35 | 9.45 | 9.35 | 8.38 | 8.34 | 6.56 |
| 13 | 8.37 | 9.18 | 9.04 | 9.24 | 7.99 | 7.75 | 6.04 |

## 5 players: excellent game percentage (less than 10 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 64.85 |   |   |   |   |   |   |
| 1 | 68.14 | 70.5 |   |   |   |   |   |
| 2 | 70.13 | 72.55 | 73.76 |   |   |   |   |
| 3 | 71.28 | 74.1 | 75.15 | 75.36 |   |   |   |
| 4 | 72.01 | 74.81 | 75.71 | 76.29 | 75.46 |   |   |
| 5 | 72.2 | 74.34 | 76.09 | 76.25 | 75.81 | 75.31 |   |
| 6 | 72.54 | 74.47 | 76.02 | 76.49 | 75.98 | 75.32 | 71.8 |
| 7 | 72.64 | 74.32 | 75.84 | 76.68 | 75.56 | 75.33 | 71.39 |
| 8 | 72.68 | 73.91 | 75.13 | 76.14 | 75.21 | 74.79 | 71.71 |
| 9 | 72.05 | 73.48 | 74.42 | 75.57 | 74.73 | 74.07 | 70.78 |
| 10 | 71.93 | 73.28 | 74.54 | 75.42 | 74.56 | 74.12 | 70.81 |
| 11 | 70.71 | 72.44 | 73.73 | 74.74 | 73.97 | 73.28 | 69.7 |
| 12 | 69.43 | 71.68 | 73.13 | 74.12 | 73.09 | 72.8 | 68.86 |
| 13 | 68.33 | 70.6 | 72.42 | 73.59 | 72.42 | 71.6 | 68.12 |

## 5 players: beat the game percentage (0 cards remaining)

| reach distance (normal →) | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| reach distance (endgame ↓) | | | | | | | |
| 0 | 7.68 |   |   |   |   |   |   |
| 1 | 9.43 | 9.11 |   |   |   |   |   |
| 2 | 10.5 | 10.05 | 11.22 |   |   |   |   |
| 3 | 10.94 | 10.99 | 11.82 | 11.34 |   |   |   |
| 4 | 11.57 | 11.29 | 11.74 | 11.48 | 11.2 |   |   |
| 5 | 12.29 | 11.97 | 11.73 | 11.91 | 10.9 | 9.51 |   |
| 6 | 11.41 | 11.92 | 11.84 | 12.22 | 11.12 | 9.38 | 7.7 |
| 7 | 11.46 | 11.05 | 11.53 | 11.54 | 10.77 | 9.63 | 7.54 |
| 8 | 10.8 | 10.5 | 10.86 | 11.35 | 10.18 | 9.16 | 6.91 |
| 9 | 10.45 | 10.17 | 10.26 | 11.07 | 10.08 | 9.12 | 6.96 |
| 10 | 10.15 | 10.08 | 10.26 | 10.79 | 9.97 | 8.91 | 6.71 |
| 11 | 9.28 | 9.57 | 9.75 | 10.21 | 9.49 | 8.28 | 6.66 |
| 12 | 8.63 | 8.87 | 9.38 | 9.59 | 8.6 | 7.93 | 6.38 |
| 13 | 8.34 | 8.41 | 8.92 | 8.89 | 7.92 | 7.45 | 6.1 |
