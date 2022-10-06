
TEST_TURN_SRC := test/test_turn.cpp src/turn.cpp 

test_turn : $(TEST_TURN_SRC) src/turn.hpp
	g++ -std=c++17 -Isrc -fsanitize=address -g -Wall $(TEST_TURN_SRC) -o $@

.PHONY: test
test : test_turn
	./test_turn