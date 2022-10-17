
TGA_SRC := \
    src/game.cpp \
    src/turn.cpp \
    src/main.cpp \

TGA_DEPENDS := \
    $(TGA_SRC) \
    src/game.hpp \
	src/turn.hpp \

.PHONY: all
all: thegameanalyzer

thegameanalyzer : $(TGA_DEPENDS)
	g++ -std=c++17 -Isrc -I../cxxopts/include -fsanitize=address -g -Wall -Werror $(TGA_SRC) -o $@ -ltbb

TEST_TURN_SRC := \
    test/test_turn.cpp \
    src/turn.cpp \

TEST_TURN_DEPENDS := $(TEST_TURN_SRC) \
    src/turn.hpp  \

test_turn : $(TEST_TURN_DEPENDS)
	g++ -std=c++17 -Isrc -fsanitize=address -g -Wall -Werror $(TEST_TURN_SRC) -o $@

.PHONY: run_test_turn
run_test_turn : test_turn
	./test_turn

TEST_GAME_SRC := \
    test/test_game.cpp \
    src/game.cpp \
    src/turn.cpp \

TEST_GAME_DEPENDS := \
    $(TEST_GAME_SRC) \
    src/game.hpp \
	src/turn.hpp \

test_game : $(TEST_GAME_DEPENDS)
	g++ -std=c++17 -Isrc -fsanitize=address -g -Wall -Werror $(TEST_GAME_SRC) -o $@ -ltbb

.PHONY: test
test : test_turn test_game
	./test_turn
	./test_game
