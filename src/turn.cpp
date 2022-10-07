#include "turn.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
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
                i = group_it->lo;
            }
            else
            {
                play.hand_mask = 1 << i; // card mask
            }
            play.pile_card_end = hand[i];
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

            Play play;
            play.piles_index = piles_index;
            play.pile_card_start = last_card;
            if (group_it != ten_groups.end())
            {
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
                play.hand_mask |= group_it->hand_mask;
                i = group_it->lo;
            }
            else
            {
                play.hand_mask = card_mask;
            }
            play.pile_card_end = hand[i];
            hand_mask |= play.hand_mask;
            plays.push_back(play);
            last_card = hand[i];
        }
        return plays;
    }

    std::string to_string(const Turn &turn)
    {
        std::ostringstream oss;
        oss << "{ piles: " << to_string(turn.piles)
            << ", hand_mask: 0x" << std::hex << turn.hand_mask
            << "\n";
        return oss.str();
    }

    static int add_piles(const Piles &piles)
    {
        return piles[0] + piles[1] - piles[2] - piles[3];
    }

    static int get_sum_of_pile_extremes(const Piles &piles)
    {
        return std::min(piles[0], piles[1]) - std::max(piles[2], piles[3]);
    }

    bool is_turn2_better(const Turn &t1, const Turn &t2, int min_cards_for_turn, int card_reach_distance)
    {
        const int t1_num_cards = get_num_cards_in_hand_mask(t1.hand_mask);
        const int t2_num_cards = get_num_cards_in_hand_mask(t2.hand_mask);
        const bool t1_has_min_cards = t1_num_cards >= min_cards_for_turn;
        const bool t2_has_min_cards = t2_num_cards >= min_cards_for_turn;
        if (t1_has_min_cards != t2_has_min_cards)
        {
            // 1. One turn has minimum cards played and the other doesn't...
            return t2_has_min_cards;
        }
        const int piles1_sum = add_piles(t1.piles);
        const int piles2_sum = add_piles(t2.piles);
        const int delta_per_card = piles2_sum - piles1_sum + (t1_num_cards - t2_num_cards) * card_reach_distance;
        if (delta_per_card < 0)
        {
            // 2. ... otherwise prefer the better points/card played...
            return true;
        }
        else if (delta_per_card == 0)
        {
            if (t1_num_cards < t2_num_cards)
            {
                // 3. ... otherwise prefer the turn with more cards...
                return true;
            }
            if (t1_num_cards == t2_num_cards)
            {
                const int sum_of_pile_extremes1 = get_sum_of_pile_extremes(t1.piles);
                const int sum_of_pile_extremes2 = get_sum_of_pile_extremes(t2.piles);
                if (sum_of_pile_extremes2 < sum_of_pile_extremes1)
                {
                    // 4. ... otherwise prefer to keep the extreme piles intact...
                    return true;
                }
                // TBD: 5. ... otherwise prefer the up/down direction with the
                // lowest combined action?
            }
        }
        return false;
    }

    static void get_plays_helper(std::vector<Plays> &vec_of_plays, const Piles &piles, size_t piles_index, Card max_card, const Hand &hand, const TenGroups &ten_groups, int min_cards_for_turn, int card_reach_distance)
    {
        auto plays = get_plays_ascending(piles[piles_index], max_card, hand, ten_groups, min_cards_for_turn, card_reach_distance);
        if (!plays.empty())
        {
            vec_of_plays.push_back(std::move(plays));
        }
    }

    static void get_plays_helper_flipped(std::vector<Plays> &vec_of_plays, const Piles &piles, size_t piles_index, Card min_card, const Hand &hand, const TenGroups &ten_groups, int min_cards_for_turn, int card_reach_distance)
    {
        flip_card(min_card);
        auto pile_card = piles[piles_index];
        flip_card(pile_card);
        auto plays = get_plays_ascending(pile_card, min_card, hand, ten_groups, min_cards_for_turn, card_reach_distance);
        if (!plays.empty())
        {
            flip_plays(plays, hand.size());
            vec_of_plays.push_back(std::move(plays));
        }
    }

    std::vector<Plays> get_vec_of_plays(const Piles &piles, const Hand &hand, int min_cards_for_turn, int card_reach_distance)
    {
        std::vector<Plays> vec_of_plays;

        // ascending piles
        {
            const Card pile0_max_card = piles[0] <= piles[1] ? piles[1] : 100;
            const Card pile1_max_card = piles[0] > piles[1] ? piles[0] : 100;
            const auto ten_groups = get_ten_groups(hand);

            get_plays_helper(vec_of_plays, piles, 0, pile0_max_card, hand, ten_groups, min_cards_for_turn, card_reach_distance);
            get_plays_helper(vec_of_plays, piles, 1, pile1_max_card, hand, ten_groups, min_cards_for_turn, card_reach_distance);
        }

        // descending piles
        {
            auto flipped_hand = hand;
            flip_hand(flipped_hand);

            const Card pile2_min_card = piles[2] >= piles[3] ? piles[3] : 1;
            const Card pile3_min_card = piles[2] < piles[3] ? piles[2] : 1;
            const auto ten_groups = get_ten_groups(flipped_hand);

            get_plays_helper_flipped(vec_of_plays, piles, 2, pile2_min_card, flipped_hand, ten_groups, min_cards_for_turn, card_reach_distance);
            get_plays_helper_flipped(vec_of_plays, piles, 3, pile3_min_card, flipped_hand, ten_groups, min_cards_for_turn, card_reach_distance);
        }
        return vec_of_plays;
    }

    struct BestPlayCalculator
    {
        Hand hand;
        Turn best_turn;
        Plays plays;
        int min_cards_for_turn = 2;
        int card_reach_distance = 1;

        void recurse(const Turn &cur_turn, size_t plays_index);
    };

    Turn find_best_turn(const Piles &piles, const Hand &hand, int min_cards_for_turn, int card_reach_distance)
    {
        const auto vec_of_plays = get_vec_of_plays(piles, hand, min_cards_for_turn, card_reach_distance);
        std::array<size_t, 4> plays_indexes = {0};

        Turn best_turn;
        for (size_t i = 0; i < vec_of_plays.size(); ++i)
        {
            Plays plays;
            plays.push_back(vec_of_plays[i][plays_indexes[i]++]);

            LEFT OFF HERE(unfortunately)

                for (size_t j = 0; j < vec_of_plays.size();)
            {
            }
        }
        BestPlayCalculator bpc;
        bpc.best_turn.piles = piles;
        bpc.min_cards_for_turn = min_cards_for_turn;
        bpc.card_reach_distance = card_reach_distance;

        if (hand.empty())
        {
            return bpc.best_turn;
        }

        Turn cur_turn(piles);
        cur_turn.num_cards_needed = min_cards_for_turn;
        bpc.recurse(0, cur_turn);

        return bpc.best_turn;
    }

    bool pile_index_is_ascending_pile(size_t pile_index)
    {
        return pile_index < 2;
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