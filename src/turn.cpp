#include "turn.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>
#include <vector>

namespace TheGameAnalyzer
{
    std::string to_string(const Hand &hand)
    {
        if (hand.empty())
        {
            return "{}";
        }
        std::ostringstream oss;
        oss << '{';
        for (const auto c : hand)
        {
            oss << static_cast<int16_t>(c) << ',';
        }
        oss << "\b}"; // \b chomps the last comma.
        return oss.str();
    }

    void flip_hand(Hand &hand)
    {
        std::reverse(hand.begin(), hand.end());
        std::transform(hand.begin(), hand.end(), hand.begin(), [](Card c)
                       { flip_card(c); return c; });
    }

    void flip_hand_mask(HandMask &hand_mask, size_t hand_size)
    {
        HandMask new_hand_mask = 0;
        // TODO this could be faster with a lookup table.
        for (size_t i = hand_size; i-- > 0;)
        {
            new_hand_mask |= ((hand_mask & 1) << i);
            hand_mask >>= 1;
        }
        hand_mask = new_hand_mask;
    }

    int get_num_cards_in_hand_mask(HandMask hand_mask)
    {
        constexpr int8_t table[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};
        return table[hand_mask];
    }

    std::string to_string(const Play &p)
    {
        std::ostringstream oss;
        oss << "{hand_mask: 0x" << std::hex << p.hand_mask
            << ", piles_index: " << p.piles_index
            << ", pile_card_start: " << p.pile_card_start
            << ", pile_card_end: " << p.pile_card_end
            << ", delta: " << p.delta << "}";
        return oss.str();
    }

    bool operator==(const Play &m1, const Play &m2)
    {
        return m1.hand_mask == m2.hand_mask &&
               m1.piles_index == m2.piles_index &&
               m1.pile_card_start == m2.pile_card_start &&
               m1.pile_card_end == m2.pile_card_end &&
               m1.delta == m2.delta;
    }
    bool operator!=(const Play &m1, const Play &m2)
    {
        return !(m1 == m2);
    }

    void flip_play(Play &play, size_t hand_size)
    {
        flip_hand_mask(play.hand_mask, hand_size);
        // Don't flip piles_index.
        flip_card(play.pile_card_start);
        flip_card(play.pile_card_end);
        // Don't flip delta.
    }

    std::string to_string(const Plays &plays)
    {
        if (plays.empty())
        {
            return "{}";
        }
        std::ostringstream oss;
        oss << '{';
        for (const auto &p : plays)
        {
            oss << to_string(p) << ',';
        }
        oss << "\b}"; // \b chomps the last comma.
        return oss.str();
    }

    void flip_plays(Plays &plays, size_t hand_size)
    {
        std::transform(plays.begin(), plays.end(), plays.begin(), [=](Play &p)
                       { flip_play(p, hand_size);
                       return p; });
    }

    bool operator==(const TenGroup &tg1, const TenGroup &tg2)
    {
        return tg1.lo == tg2.lo &&
               tg1.hi == tg2.hi &&
               tg1.hand_mask == tg2.hand_mask;
    }

    bool operator!=(const TenGroup &tg1, const TenGroup &tg2)
    {
        return !(tg1 == tg2);
    }

    std::string to_string(const TenGroup &tg)
    {
        std::ostringstream oss;
        oss << "{lo: " << tg.lo
            << ", hi: " << tg.hi
            << ", hand_mask: 0x" << std::hex << tg.hand_mask;
        return oss.str();
    }

    bool operator==(const TenGroups &tg1, const TenGroups &tg2)
    {
        return tg1.groups == tg2.groups &&
               tg1.groups_hand_mask == tg2.groups_hand_mask;
    }

    bool operator!=(const TenGroups &tg1, const TenGroups &tg2)
    {
        return !(tg1 == tg2);
    }

    std::string to_string(const TenGroups &ten_groups)
    {
        std::ostringstream oss;
        oss << "{ groups: {";
        if (ten_groups.groups.empty())
        {
            oss << "}";
        }
        else
        {
            for (const auto &g : ten_groups.groups)
            {
                oss << to_string(g) << ',';
            }
            oss << "\b}"; // \b chomps the last comma.
        }
        oss << ", hand_mask: 0x" << std::hex << ten_groups.groups_hand_mask << "}";
        return oss.str();
    }

    TenGroups get_ten_groups(const Hand &hand)
    {
        TenGroups ten_groups;
        for (size_t i = 0; i < hand.size(); ++i)
        {
            const HandMask card_mask = 1 << i;
            if ((card_mask & ten_groups.groups_hand_mask) != 0)
            {
                continue;
            }
            Card num_to_find = hand[i] + 10;
            TenGroup tg{i, i, card_mask};
            for (size_t j = i + 1; j < hand.size(); ++j)
            {
                if (hand[j] > num_to_find)
                {
                    break;
                }
                if (hand[j] == num_to_find)
                {
                    num_to_find += 10;
                    tg.hand_mask |= 1 << j;
                    tg.hi = j;
                }
            }
            if (tg.hand_mask != card_mask)
            {
                ten_groups.groups_hand_mask |= tg.hand_mask;
                ten_groups.push_back(tg);
            }
        }
        return ten_groups;
    }

    std::string to_string(const Piles &piles)
    {
        assert(piles.size() == 4);
        std::string s;
        s += "{";
        s += std::to_string(piles[0]);
        s += ", ";
        s += std::to_string(piles[1]);
        s += ", ";
        s += std::to_string(piles[2]);
        s += ", ";
        s += std::to_string(piles[3]);
        s += "}";
        return s;
    }

    Plays get_plays_ascending(Card pile_card, size_t piles_index, const Hand &hand,
                              const TenGroups &ten_groups, int min_cards_for_turn,
                              int card_reach_distance)
    {
        Plays plays;
        HandMask hand_mask = 0;
        Card last_card = pile_card;
        const Card pile_card_minus_10 = pile_card - 10;
        size_t i = static_cast<size_t>(std::find_if(hand.begin(), hand.end(), [=](const auto c)
                                                    { return c >= pile_card_minus_10; }) -
                                       hand.begin());
        if (i < hand.size() && hand[i] == pile_card_minus_10)
        {
            Play play;
            play.piles_index = piles_index;
            play.pile_card_start = last_card;
            auto group_it = std::find_if(ten_groups.begin(), ten_groups.end(), [=](const auto &g)
                                         { return i == g.hi; });
            if (group_it != ten_groups.end())
            {
                play.hand_mask = group_it->hand_mask;
                play.pile_card_end = hand[group_it->lo];
                i = group_it->lo;
            }
            else
            {
                play.hand_mask = 1 << i; // card mask
                play.pile_card_end = hand[i];
            }
            hand_mask |= play.hand_mask;
            plays.push_back(play);
            last_card = hand[i];
        }
        else
        {
            i = static_cast<size_t>(std::find_if(hand.begin(), hand.end(), [=](const Card c)
                                                 { return c > pile_card; }) -
                                    hand.begin());
        }

        for (; i < hand.size(); ++i)
        {
            const unsigned card_mask = 1 << i;

            // Skip cards that are already in a play.
            if ((card_mask & hand_mask) != 0)
            {
                continue;
            }

            auto group_it = std::find_if(ten_groups.begin(), ten_groups.end(), [=](const auto &g)
                                         { return (g.lo <= i && i <= g.hi) && (hand_mask & g.hand_mask) == 0; });
            // Bail if we have enough cards and not enough small enough jump to play an extra.
            const int card_delta = hand[i] - last_card;
            if (get_num_cards_in_hand_mask(hand_mask) >= min_cards_for_turn)
            {
                if (card_delta > card_reach_distance)
                {
                    break;
                }
                // If the card is within the delta but the start of a group then skip it.
                if (group_it != ten_groups.end() && group_it->lo == hand[i])
                {
                    break;
                }
            }

            LEFT OFF HERE !!!!Play play;
            = {card_mask, 1, i, i};
            play.piles_index = piles_index;
            play.pile_card_start = last_card;
            if (group_it != ten_groups.end())
            {
                play *group_it;
                // Add in all proceeding unmasked cards in this group to the group.
                for (size_t j = i; j < group_it->hi; ++j)
                {
                    const unsigned next_card_mask = 1 << j;
                    if ((next_card_mask & (hand_mask | ten_groups.groups_hand_mask)) == 0)
                    {
                        play.hand_mask |= next_card_mask;
                    }
                }
                i = group_it->lo;
            }
            hand_mask |= play.hand_mask;
            plays.push_back(play);
            last_card = hand[i];
        }
        return plays;
    }

    // int get_turn_score(const Piles &piles, int cards_needed, int cards_played)
    // {
    //     assert(cards_played >= 0);
    //     const int additional_score = 100 * std::min(cards_played, cards_needed);
    //     return additional_score + 198 - piles[0] - piles[1] + piles[2] + piles[3];
    // }

    struct BestPlayCalculator
    {
        Hand hand;
        Turn best_turn;
        Plays plays;
        int min_cards_for_turn = 2;
        int card_reach_distance = 1;

        void recurse(const Turn &cur_turn, size_t plays_index);
    };

    Turn do_best_turn(const Piles &piles, const Hand &hand, int min_cards_for_turn, int card_reach_distance)
    {
        BestPlayCalculator bpc;
        bpc.best_turn.piles = piles;
        bpc.min_cards_for_turn = min_cards_for_turn;
        bpc.card_reach_distance = card_reach_distance;

        if (hand.empty())
        {
            return bpc.best_turn;
        }

        const auto ten_groups = get_ten_groups(hand);
        bpc.pile_moves[0] = get_plays_ascending(piles[0], hand, ten_groups, min_cards_for_turn, card_reach_distance);
        bpc.pile_moves[1] = get_plays_ascending(piles[1], hand, ten_groups, min_cards_for_turn, card_reach_distance);
        const auto flipped_hand = flip_it_and_reverse_it(hand);
        const auto flipped_movegroups = flip_it_and_reverse_it(ten_groups);
        bpc.pile_moves[2] = get_plays_ascending(flip_it_and_reverse_it(piles[2]), flipped_movegroups, flipped_hand, min_cards_for_turn, card_reach_distance);
        bpc.pile_moves[3] = get_plays_ascending(flip_it_and_reverse_it(piles[3]), flipped_movegroups, flipped_hand, min_cards_for_turn, card_reach_distance);

        Turn cur_turn(piles);
        cur_turn.num_cards_needed = min_cards_for_turn;
        bpc.recurse(0, cur_turn);

        return bpc.best_turn;
    }

    bool pile_index_is_ascending_pile(size_t pile_index)
    {
        return pile_index < 2;
    }

    constexpr bool operator<(const Turn &t1, const Turn &t2)
    {
        if (t1.num_cards_needed != t2.num_cards_needed)
        {
            return t1.num_cards_needed > t2.num_cards_needed;
        }
        const int score1 = t1.piles_sum + t1.extra_play_points;
        const int score2 = t2.piles_sum + t2.extra_play_points;
        if (score1 != score2)
        {
            return score1 < score2;
        }
        return t1.extra_play_points < t2.extra_play_points;
    }

    bool Turn::do_move(const Play &move, size_t pile_index, const Hand &hand, int card_reach_distance)
    {
        if ((move.hand_mask & hand_mask) != 0)
        {
            return false;
        }
        hand_mask |= move.hand_mask;
        if (pile_index_is_ascending_pile(pile_index))
        {
            piles_sum += piles[pile_index] - hand[move.lo];
            piles[pile_index] = hand[move.lo];
        }
        else
        {
            piles_sum += piles[pile_index] - hand[move.hi];
            piles[pile_index] = hand[move.hi];
        }
        if (num_cards_needed == 0)
        {
            extra_play_points += card_reach_distance;
        }
        else
        {
            num_cards_needed -= move.num_cards;
            num_cards_needed = std::max(0, num_cards_needed);
        }
        return true;
    }

    void BestPlayCalculator::recurse(size_t piles_index, const Turn &cur_turn)
    {
        for (size_t i = piles_index; i < cur_turn.piles.size(); ++i)
        {
            const size_t mi = moves_indexes[i];
            const Moves &moves = pile_moves[i];
            if (mi < moves.size())
            {
                Turn next_turn = cur_turn;
                if (next_turn.do_move(moves[mi], i, hand, card_reach_distance))
                {
                    if (cur_turn.num_cards_needed == 0 &&
                        next_turn < best_turn)
                    {
                        continue;
                    }
                    if (best_turn < cur_turn)
                    {
                        best_turn = cur_turn;
                    }
                    ++moves_indexes[i];
                    recurse(i, next_turn);
                    --moves_indexes[i];
                }
            }
        }
    }
} // namespace TheGameAnalyzer