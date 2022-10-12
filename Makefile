
TGA_SRC := \
    src/games.cpp \
    src/game.cpp \
    src/turn.cpp \
    src/main.cpp \

TGA_DEPENDS := \
    $(TGA_SRC) \
    src/game.hpp \
	src/games.hpp \
	src/turn.hpp \

.PHONY: all
all: thegameanalyzer

thegameanalyzer : $(TGA_DEPENDS)
	g++ -std=c++17 -Isrc -fsanitize=address -g -Wall $(TGA_SRC) -o $@

TEST_TURN_SRC := \
    test/test_turn.cpp \
    src/test.cpp \

TEST_TURN_DEPENDS := $(TEST_TURN_SRC) \
    src/turn.hpp  \

test_turn : $(TEST_TURN_DEPENDS)
	g++ -std=c++17 -Isrc -fsanitize=address -g -Wall $(TEST_TURN_SRC) -o $@

TEST_GAMES_SRC := \
    test/test_games.cpp \
    test/test_game.cpp \
    test/test_turn.cpp \

TEST_GAMES_DEPENDS := \
    $(TEST_GAMES_SRC) \
    src/game.hpp \
	src/games.hpp \
	src/turn.hpp \

test_games : $(TEST_GAMES_DEPENDS)
	g++ -std=c++17 -Isrc -fsanitize=address -g -Wall $(TEST_GAMES_SRC) -o $@

.PHONY: test
test : test_turn test_games
	./test_turn
	./test_games
