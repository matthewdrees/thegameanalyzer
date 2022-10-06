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

    TestCase test_cases[] = {
        {{1}, {100}},
        {{2, 99}, {99, 2}},
        {{2, 3, 4}, {97, 98, 99}},
        {{5, 10, 15, 20, 30, 50, 67}, {34, 51, 61, 71, 81, 86, 91, 96}},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        auto act = tc.hand;
        flip_hand(act);
        if (tc.exp != act)
        {
            ++num_fails;
            std::cerr << "FAIL, " << __FUNCTION__
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

    TestCase test_cases[] = {
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
            std::cerr << "FAIL, " << __FUNCTION__
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
            std::cerr << "FAIL, " << __FUNCTION__
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

    TestCase test_cases[] = {
        {{2}, {{}, 0}},
        {{2, 12}, {{{0, 1, 0x3}}, 0x3}},
        {{2, 12, 22}, {{{0, 2, 0x7}}, 0x7}},
        {{2, 6, 12, 17, 32, 40, 42, 52, 62, 72, 82}, {{{0, 2, 0x5}, {4, 10, 0x7d0}}, 0x7d5}},
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
            std::cerr << "FAIL, " << __FUNCTION__
                      << "(hand: " << to_string(tc.hand) << ")\n"
                      << ", exp: " << to_string(tc.exp) << '\n'
                      << ", act: " << to_string(act) << '\n';
        }
    }
    return num_fails;
}

int test_do_best_turn_2_cards_1_over()
{
    struct TestCase
    {
        Piles piles;
        std::vector<Card> cards;
        Piles exp_piles;
        std::vector<Card> exp_cards;
    };

    TestCase test_cases[] = {
        {{1, 1, 100, 100}, {8, 13, 26, 43, 51, 56, 63, 81}, {13, 1, 100, 100}, {26, 43, 51, 56, 63, 81}},
        {{13, 1, 1, 1}, {26, 43, 51, 56, 61, 63, 73, 81}, {13, 1, 73, 100}, {26, 43, 51, 56, 61}},
        {{13, 1, 73, 100}, {14, 22, 24, 26, 43, 51, 56, 61}, {14, 1, 73, 100}, {26, 43, 51, 56, 61}},
        {{14, 1, 73, 100}, {21, 26, 37, 43, 51, 56, 61}, {26, 1, 73, 100}, {37, 41, 43, 51, 56, 61}},
        {{26, 1, 73, 100}, {37, 41, 43, 51, 56, 61, 65, 66}, {26, 1, 65, 100}, {37, 41, 43, 51, 56, 61}},
        {{26, 1, 65, 100}, {3, 17, 37, 41, 43, 51, 56, 61}, {26, 1, 61, 100}, {3, 17, 37}}, // interesting.
        {{26, 1, 61, 100}, {3, 15, 17, 31, 37, 58, 70, 99}, {26, 3, 61, 99}, {15, 17, 31, 37, 58, 70}},
        {{26, 3, 61, 99}, {15, 17, 31, 37, 58, 70}, {31, 3, 61, 99}, {15, 17, 37, 58, 70, 75}},
    };

    int num_fails = 0;
    // for (const auto &tc : test_cases)
    {
        // Turn do_best_turn(tc.piles, tc.cards)
    }
    return num_fails;
}

int test_get_moves_ascending_2_cards_1_over()
{
    struct TestCase
    {
        Card pile_card;
        std::vector<Card> cards;
        Moves moves;
    };

    TestCase test_cases[] = {
        {5, {}, {}},
        {5, {4}, {}},
        {5, {6}, {{0x1, 1, 0, 0}}},
        {2, {3, 17, 37, 41, 43, 51, 56, 61}, {{0x1, 1, 0, 0}, {0x2, 1, 1, 1}}},
        {26, {3, 17, 37, 41, 43, 51, 56, 61}, {{0x4, 1, 2, 2}, {0xf8, 5, 3, 7}}},
        {40, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xf8, 5, 3, 7}}},
        {42, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xf8, 5, 3, 7}}},
        {44, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xe8, 4, 3, 7}}},
        {43, {3, 17, 37, 41, 42, 51, 56, 61}, {{0xe8, 4, 3, 7}, {0x10, 1, 4, 4}}},
        {54, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xe8, 4, 3, 7}}},
        {57, {3, 17, 37, 41, 43, 51, 56, 61}, {{0xa8, 3, 3, 7}}},
        {24, {25, 30, 35, 40}, {{0x5, 2, 0, 2}}},
        {29, {25, 30, 35, 40}, {{0x5, 2, 0, 2}}},
        {34, {25, 30, 35, 40}, {{0x5, 2, 0, 2}}},
        {36, {25, 30, 35, 40}, {{0xa, 2, 1, 3}}},
        {13, {2, 10, 12, 20}, {{0xa, 2, 1, 3}}},
        {13, {2, 11, 12, 21}, {{0xa, 2, 1, 3}, {0x5, 2, 0, 2}}},
        {10, {2, 11, 12, 21}, {{0x5, 2, 0, 2}}},
        {31, {2, 11, 12, 20, 21, 29, 30, 39}, {
                                                  {0x12, 2, 1, 4},
                                                  {0x5, 2, 0, 2},
                                              }},
        {32, {2, 11, 12, 20, 21, 29, 30, 39}, {
                                                  {0xa0, 2, 5, 7},
                                                  {0x48, 2, 3, 6},
                                                  {0x12, 2, 1, 4},
                                                  {0x5, 2, 0, 2},
                                              }},
    };
    int num_fails = 0;
    for (const auto &tc : test_cases)
    {
        const auto movegroups = get_tengroups(tc.cards);
        const auto moves = get_moves_ascending(tc.pile_card, movegroups, tc.cards, 2, 1);
        if (tc.moves != moves)
        {
            ++num_fails;
            std::cerr << "FAIL, " << __FUNCTION__
                      << "(pile_card: " << static_cast<int16_t>(tc.pile_card)
                      << ", cards: " << to_string(tc.cards) << ")\n"
                      << "- exp moves: " << to_string(tc.moves) << '\n'
                      << "- act moves: " << to_string(moves) << '\n';
        }
    }
    return num_fails;
}

// int test_get_turn_score()
// {
//     struct TestCase
//     {
//         Piles piles;
//         int num_cards_needed;
//         int num_cards_played;
//         int expected;
//     };
//     const TestCase test_cases[] = {
//         {{1, 1, 100, 100}, 2, 0, 396},
//         {{2, 1, 100, 100}, 2, 0, 395},
//         {{1, 2, 100, 100}, 2, 0, 395},
//         {{1, 1, 99, 100}, 2, 0, 395},
//         {{1, 1, 100, 99}, 2, 0, 395},
//         {{49, 50, 51, 52}, 2, 0, 395},
//         {{99, 98, 2, 3}, 2, 0, 202},
//         {{99, 98, 2, 3}, 2, 1, 106},
//         {{99, 98, 2, 3}, 2, 2, 206},
//         {{99, 98, 2, 3}, 2, 3, 206},
//         {{99, 98, 2, 3}, 1, 0, 6},
//         {{99, 98, 2, 3}, 1, 1, 106},
//         {{99, 98, 2, 3}, 1, 2, 106},
//     };
//     int num_fails = 0;
//     for (const auto &tc : test_cases)
//     {
//         const auto actual = get_turn_score(tc.piles, tc.num_cards_needed, tc.num_cards_played);
//         if (tc.expected != actual)
//         {
//             ++num_fails;
//             std::cerr << "FAIL, " << __FUNCTION__
//                       << "(piles: " << to_string(tc.piles)
//                       << ", num_cards_needed: " << tc.num_cards_needed
//                       << ", num_cards_played: " << tc.num_cards_played << ")"
//                       << ", exp: " << tc.expected
//                       << ", act: " << actual << "\n";
//         }
//     }
//     return num_fails;
// }

int test_turn_compare()
{
    // TODO Turn::operator<
}

int main()
{
    int num_fails = test_flip_hand() +
                    test_flip_hand_mask() +
                    test_get_num_cards_in_hand_mask() +
                    test_get_ten_groups();

    return num_fails != 0;
}