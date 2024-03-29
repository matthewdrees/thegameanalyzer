#include "turn.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace TheGameAnalyzer;

int test_flip_hand()
{
    struct TestCase
    {
        Hand hand;
        Hand exp;
    };

    const TestCase test_cases[] = {
        {{1}, {100}},
        {{2, 99}, {2, 99}},
        {{2, 3, 4}, {97, 98, 99}},
        {{5, 10, 15, 20, 30, 50, 67}, {34, 51, 71, 81, 86, 91, 96}},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        auto act = tc.hand;
        flip_hand(act);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(hand: " << to_string(tc.hand) << ")"
                      << ", exp: " << to_string(tc.exp)
                      << ", act: " << to_string(act) << '\n';
        }
    }
    return num_fails;
}

int test_flip_hand_mask()
{
    struct TestCase
    {
        HandMask hand_mask;
        size_t hand_size;
        HandMask exp;
    };

    const TestCase test_cases[] = {
        {0b0, 0, 0b0},
        {0b00000000, 8, 0b00000000},
        {0b00000001, 8, 0b10000000},
        {0b00000010, 8, 0b01000000},
        {0b00000100, 8, 0b00100000},
        {0b00001000, 8, 0b00010000},
        {0b00010000, 8, 0b00001000},
        {0b00100000, 8, 0b00000100},
        {0b01000000, 8, 0b00000010},
        {0b10000000, 8, 0b00000001},
        {0b01010101, 8, 0b10101010},

        {0b00000001, 7, 0b01000000},
        {0b00000010, 7, 0b00100000},
        {0b00000100, 7, 0b00010000},
        {0b00001000, 7, 0b00001000},
        {0b00010000, 7, 0b00000100},
        {0b00100000, 7, 0b00000010},
        {0b01000000, 7, 0b00000001},
        {0b10000000, 7, 0b00000000},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        HandMask act = tc.hand_mask;
        flip_hand_mask(act, tc.hand_size);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(hand_mask: 0x" << std::hex << tc.hand_mask
                      << ", hand_size: " << tc.hand_size
                      << "), exp: " << tc.exp
                      << "act: " << act << "\n";
        }
    }
    return num_fails;
}

static int count_bits(HandMask hand_mask)
{
    int num_bits = 0;
    while (hand_mask != 0)
    {
        if ((hand_mask & 1))
        {
            ++num_bits;
        }
        hand_mask >>= 1;
    }
    return num_bits;
}

int test_get_num_cards_in_hand_mask()
{
    int num_fails = 0;
    for (unsigned i = 0; i < 256; ++i)
    {
        HandMask hand_mask = static_cast<HandMask>(i);
        int exp = count_bits(hand_mask);
        int act = get_num_cards_in_hand_mask(hand_mask);
        if (exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(hand_mask: 0x" << std::hex << hand_mask
                      << "), exp: " << exp
                      << "act: " << act << "\n";
        }
    }
    return num_fails;
}

int test_get_ten_groups()
{
    struct TestCase
    {
        Hand hand;
        TenGroups exp;
    };

    const TestCase test_cases[] = {
        {{2}, {{}, 0}},
        {{2, 12}, {{{0, 1, 0x3}}, 0x3}},
        {{2, 12, 22}, {{{0, 2, 0x7}}, 0x7}},
        {{2, 6, 12, 17, 32, 40, 42, 52}, {{{0, 2, 0x5}, {4, 7, 0xd0}}, 0xd5}},
        {{2, 11, 22}, {{}, 0}},
        {{2, 13, 22}, {{}, 0}},
        {{25, 30, 35, 40}, {{{0, 2, 0x5}, {1, 3, 0xa}}, 0xf}},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const auto act = get_ten_groups(tc.hand);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(hand: " << to_string(tc.hand) << ")\n"
                      << ", exp: " << to_string(tc.exp) << '\n'
                      << ", act: " << to_string(act) << '\n';
        }
    }
    return num_fails;
}

int test_get_plays_ascending_2_cards_1_over()
{
    struct TestCase
    {
        Card pile_card;
        Hand hand;
        Plays exp;
    };

    const TestCase test_cases[] = {
        {5, {}, {}},
        {5, {4}, {}},
        {5, {6}, {{0x1, 1, 5, 6, 1}}},
        {2, {3, 17, 37, 41, 43, 51, 56, 61}, {{0x1, 1, 2, 3, 1}, {0x2, 1, 3, 17, 14}}},
        {26, {3, 17, 37, 41, 43, 51, 56, 61}, {{0x4, 1, 26, 37, 11}, {0xf8, 1, 37, 41, 4}}},
        {40, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xf8, 1, 40, 41, 1}}},
        {42, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xf8, 1, 42, 41, -1}}},
        {44, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xe8, 1, 44, 41, -3}}},
        {43, {3, 17, 37, 41, 42, 51, 56, 61}, {{0xe8, 1, 43, 41, -2}, {0x10, 1, 41, 42, 1}}},
        {54, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xe8, 1, 54, 41, -13}}},
        {57, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xa8, 1, 57, 41, -16}}},
        {24, {25, 30, 35, 40}, {{0x5, 1, 24, 25, 1}}},
        {29, {25, 30, 35, 40}, {{0x5, 1, 29, 25, -4}}},
        {34, {25, 30, 35, 40}, {{0x5, 1, 34, 25, -9}}},
        {36, {25, 30, 35, 40}, {{0xa, 1, 36, 30, -6}}},
        {13, {2, 10, 12, 20}, {{0xa, 1, 13, 10, -3}}},
        {13, {2, 11, 12, 21}, {{0xa, 1, 13, 11, -2}, {0x5, 1, 11, 2, -9}}},
        {10, {2, 11, 12, 21}, {{0x5, 1, 10, 2, -8}}},
        {31, {2, 11, 12, 20, 21, 29, 30, 39}, {
                                                  {0x12, 1, 31, 11, -20},
                                                  {0x5, 1, 11, 2, -9},
                                              }},
        {32, {2, 11, 12, 20, 21, 29, 30, 39}, {
                                                  {0xa0, 1, 32, 29, -3},
                                                  {0x48, 1, 29, 20, -9},
                                                  {0x12, 1, 20, 11, -9},
                                                  {0x5, 1, 11, 2, -9},
                                              }},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const auto ten_groups = get_ten_groups(tc.hand);
        const auto act = get_plays_ascending(tc.pile_card, 100, 1, tc.hand, ten_groups, 2, 1);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(pile_card: " << static_cast<int16_t>(tc.pile_card)
                      << ", hand: " << to_string(tc.hand) << ")\n"
                      << "exp: " << to_string(tc.exp) << '\n'
                      << "act: " << to_string(act) << '\n';
        }
    }
    return num_fails;
}

int test_turn_compare()
{
    struct TestCase
    {
        Turn t1;
        Turn t2;
        int min_cards_for_turn;
        int card_reach_distance;
        bool exp;
    };
    const TestCase test_cases[] = {
        // 1. One has min cards, the other doesn't
        {
            {{1, 1, 100, 90}, 0x1, 8, {0, 0, 0, 0}, false},
            {{1, 1, 100, 100}, 0x3, 9, {0, 0, 0, 0}, false},
            2,
            1,
            true,
        },
        {
            {{1, 1, 100, 100}, 0x3, 9, {0, 0, 0, 0}, false},
            {{1, 1, 100, 90}, 0x1, 8, {0, 0, 0, 0}, false},
            2,
            1,
            false,
        },
        // 2. smaller delta
        {
            {{1, 1, 100, 100}, 0x3, 1, {0, 0, 0, 0}, false},
            {{4, 1, 100, 100}, 0x7, 2, {0, 0, 0, 0}, false},
            2,
            1,
            false,
        },
        {
            {{1, 1, 100, 100}, 0x3, 2, {0, 0, 0, 0}, false},
            {{1, 2, 100, 100}, 0x7, 1, {0, 0, 0, 0}, false},
            2,
            1,
            true,
        },
        // 3. reached for a group
        {
            {{1, 3, 100, 100}, 0x7, 8, {0, 0, 0, 0}, true},
            {{1, 1, 98, 100}, 0x3, 8, {0, 0, 0, 0}, false},
            2,
            1,
            true,
        },
        {
            {{1, 3, 100, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            {{1, 1, 98, 100}, 0x3, 8, {0, 0, 0, 0}, true},
            2,
            1,
            false,
        },
        // 4. turn that used more cards
        {
            {{1, 3, 100, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            {{1, 1, 98, 100}, 0x3, 8, {0, 0, 0, 0}, false},
            2,
            1,
            false,
        },
        {
            {{1, 3, 100, 100}, 0x3, 8, {0, 0, 0, 0}, false},
            {{1, 1, 98, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            2,
            1,
            true,
        },
        // 5. keep extremes intact
        {
            {{5, 10, 98, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            {{7, 10, 100, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            2,
            1,
            false,
        },
        {
            {{7, 10, 100, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            {{5, 10, 98, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            2,
            1,
            true,
        },
        // 5. Tiebreaker fall-thru
        {
            {{1, 1, 98, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            {{1, 3, 100, 100}, 0x7, 8, {0, 0, 0, 0}, false},
            2,
            1,
            false,
        },
        {
            {{5, 10, 90, 98}, 0x7, 8, {0, 0, 0, 0}, true},
            {{7, 10, 90, 100}, 0x7, 8, {0, 0, 0, 0}, true},
            2,
            1,
            false,
        },
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const TurnCompare turn_compare{tc.min_cards_for_turn};
        const auto act = turn_compare(tc.t1, tc.t2);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(t1: " << to_string(tc.t1)
                      << ",t2: " << to_string(tc.t2)
                      << ", min_cards: " << tc.min_cards_for_turn
                      << ", crd: " << tc.card_reach_distance << ")"
                      << ", exp: " << tc.exp
                      << ", act: " << act << "\n";
        }
    }
    return num_fails;
}

int test_find_best_turn_2_1()
{
    struct TestCase
    {
        Piles piles;
        Hand hand;
        Turn exp;
    };

    const TestCase test_cases[] = {
        {{1, 8, 100, 100}, {6, 11, 20, 24, 51, 53, 57, 92}, {{6, 11, 100, 100}, 0x3, 8, {1, 1, 0, 0}, false}},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const auto act = find_best_turn(tc.piles, tc.hand, 2, 1);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << __FILE__ << ":" << __LINE__ << ". FAIL, " << __FUNCTION__
                      << "(piles: " << to_string(tc.piles)
                      << ",hand: " << to_string(tc.hand)
                      << ", min_cards: " << 2
                      << ", crd: " << 1 << ")"
                      << ", exp: " << to_string(tc.exp)
                      << ", act: " << to_string(act) << "\n";
        }
    }
    return num_fails;
}

int main()
{
    const int num_fails = test_flip_hand() +
                          test_flip_hand_mask() +
                          test_get_num_cards_in_hand_mask() +
                          test_get_ten_groups() +
                          test_get_plays_ascending_2_cards_1_over() +
                          test_turn_compare() +
                          test_find_best_turn_2_1();

    return num_fails != 0;
}