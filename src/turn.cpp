#include "turn.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <sstream>
#include <vector>

namespace TheGameAnalyzer
{
    std::string to_string(const Hand &hand)
    {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto c : hand)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                oss << ",";
            }
            oss << static_cast<int16_t>(c);
        }
        oss << "}";
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
        oss << "{hand_mask: 0x" << std::hex << p.hand_mask << std::dec
            << ", piles_index: " << p.piles_index
            << ", pile_card_start: " << p.pile_card_start
            << ", pile_card_end: " << p.pile_card_end
            << ", delta: " << p.delta << "}";
        return oss.str();
    }

    bool operator==(const Play &p1, const Play &p2)
    {
        return p1.hand_mask == p2.hand_mask &&
               p1.piles_index == p2.piles_index &&
               p1.pile_card_start == p2.pile_card_start &&
               p1.pile_card_end == p2.pile_card_end &&
               p1.delta == p2.delta;
    }
    bool operator!=(const Play &p1, const Play &p2)
    {
        return !(p1 == p2);
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
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto &p : plays)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                oss << ",";
            }
            oss << to_string(p);
        }
        oss << "}";
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
            << ", hand_mask: 0x" << std::hex << tg.hand_mask << std::dec;
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
        bool first = true;
        for (const auto &g : ten_groups.groups)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                oss << ",";
            }
            oss << to_string(g);
        }
        oss << "}, hand_mask: 0x" << std::hex << ten_groups.groups_hand_mask << std::dec << "}";
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
            if (tg.lo != tg.hi)
            {
                ten_groups.groups_hand_mask |= tg.hand_mask;
                ten_groups.push_back(tg);
            }
        }
        return ten_groups;
    }

    std::string to_string(const Piles &piles)
    {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto c : piles)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                oss << ",";
            }
            oss << static_cast<int16_t>(c);
        }
        oss << "}";
        return oss.str();
    }

    Plays get_plays_ascending(Card pile_card, Card max_card, size_t piles_index, const Hand &hand,
                              const TenGroups &ten_groups, int min_cards_for_turn,
                              int card_reach_distance)
    {
        Plays plays;
        HandMask hand_mask = 0;
        Card last_card = pile_card;
        const Card pile_card_minus_10 = pile_card - 10;
        size_t i = static_cast<size_t>(std::find(hand.begin(), hand.end(), pile_card_minus_10) - hand.begin());
        if (i < hand.size())
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
            play.delta = play.pile_card_end - play.pile_card_start;
            hand_mask |= play.hand_mask;
            plays.push_back(std::move(play));
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
            if (max_card <= hand[i])
            {
                break;
            }
            const HandMask card_mask = 1 << i;

            // Skip cards that are already in a play.
            if ((card_mask & hand_mask) != 0)
            {
                continue;
            }

            auto group_it = std::find_if(ten_groups.begin(), ten_groups.end(), [=](const auto &g)
                                         { return (g.lo <= i && i <= g.hi) && (hand_mask & g.hand_mask) == 0; });
            if (get_num_cards_in_hand_mask(hand_mask) >= min_cards_for_turn)
            {
                // Bail if we have enough cards and not enough small enough jump to play an extra.
                const int card_delta = hand[i] - last_card;
                if (card_delta > card_reach_distance)
                {
                    break;
                }
                // If the card is within the delta but the start of a group then skip it.
                if (group_it != ten_groups.end() && group_it->lo == i)
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
                play.hand_mask |= group_it->hand_mask;
                i = group_it->lo;
            }
            else
            {
                play.hand_mask = card_mask;
            }
            play.pile_card_end = hand[i];
            play.delta = play.pile_card_end - play.pile_card_start;
            hand_mask |= play.hand_mask;
            plays.push_back(std::move(play));
            last_card = hand[i];
        }
        return plays;
    }

    std::string to_string(PilesIndexes piles_indexes)
    {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto pi : piles_indexes)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                oss << ",";
            }
            oss << pi;
        }
        oss << "}";
        return oss.str();
    }

    bool operator==(const Turn &t1, const Turn &t2)
    {
        return t1.piles == t2.piles &&
               t1.hand_mask == t2.hand_mask &&
               t1.delta == t2.delta &&
               t1.piles_indexes == t2.piles_indexes &&
               t1.reached_for_group == t2.reached_for_group;
    }
    bool operator!=(const Turn &t1, const Turn &t2)
    {
        return !(t1 == t2);
    }

    std::string to_string(const Turn &t)
    {
        std::ostringstream oss;
        oss << "{ piles: " << to_string(t.piles)
            << ", hand_mask: 0x" << std::hex << t.hand_mask << std::dec
            << ", delta: " << t.delta
            << ", piles_indexes: " << to_string(t.piles_indexes)
            << ", reached_for_group: " << t.reached_for_group
            << "\n";
        return oss.str();
    }

    static int get_sum_of_pile_extremes(const Piles &piles)
    {
        return std::min(piles[0], piles[1]) - std::max(piles[2], piles[3]);
    }

    using PilesOfPlays = std::array<Plays, 4>;

    PilesOfPlays get_piles_of_plays(const Piles &piles, const Hand &hand, int min_cards_for_turn, int card_reach_distance)
    {
        PilesOfPlays piles_of_plays;
        Piles bound_cards = {100, 100, 1, 1};
        // ascending piles
        {
            if (piles[0] <= piles[1])
            {
                bound_cards[0] = piles[1];
            }
            else
            {
                bound_cards[1] = piles[0];
            }
            const auto ten_groups = get_ten_groups(hand);

            for (size_t i = 0; i < 2; ++i)
            {
                piles_of_plays[i] = get_plays_ascending(piles[i], bound_cards[i], i, hand, ten_groups, min_cards_for_turn, card_reach_distance);
            }
        }

        // descending piles
        {
            auto flipped_hand = hand;
            flip_hand(flipped_hand);
            if (piles[2] <= piles[3])
            {
                bound_cards[3] = piles[2];
            }
            else
            {
                bound_cards[2] = piles[3];
            }
            const auto ten_groups = get_ten_groups(flipped_hand);

            for (size_t i = 2; i < 4; ++i)
            {
                flip_card(bound_cards[i]);
                auto pile_card = piles[i];
                flip_card(pile_card);
                piles_of_plays[i] = get_plays_ascending(pile_card, bound_cards[i], i, flipped_hand, ten_groups, min_cards_for_turn, card_reach_distance);
                flip_plays(piles_of_plays[i], hand.size());
            }
        }
        return piles_of_plays;
    }

    // Get the piles_index for the next smallest possible play, if possible.
    std::optional<size_t> get_next_min_play_piles_index(const PilesOfPlays &piles_of_plays, const PilesIndexes &piles_indexes, HandMask hand_mask)
    {
        std::optional<size_t> piles_index;
        for (size_t pi = 0; pi < piles_of_plays.size(); ++pi)
        {
            const auto &plays = piles_of_plays[pi];
            const size_t play_index = piles_indexes[pi];
            if (play_index >= plays.size())
            {
                continue;
            }
            const auto &play = plays[play_index];
            if ((play.hand_mask & hand_mask) != 0)
            {
                continue;
            }
            if (!piles_index)
            {
                piles_index = pi;
                continue;
            }
            const auto &prev_play = piles_of_plays[*piles_index][piles_indexes[*piles_index]];
            if (play.delta != prev_play.delta)
            {
                if (play.delta < prev_play.delta)
                {
                    piles_index = pi;
                }
            }
            else if (prev_play.is_group_reach() && !play.is_group_reach())
            {
                piles_index = pi;
            }
        }
        return piles_index;
    }

    void update_turn_from_play(const Play &play, size_t pi, Turn &t)
    {
        assert(t.piles[pi] == play.pile_card_start && "Pile card not expected for play.");
        t.piles[pi] = play.pile_card_end;
        t.hand_mask |= play.hand_mask;
        t.delta += play.delta;
        ++t.piles_indexes[pi];
        t.reached_for_group = t.reached_for_group || play.is_group_reach();
    }

    bool TurnCompare::operator()(const Turn &t1, const Turn &t2) const
    {
        const int t1_num_cards = get_num_cards_in_hand_mask(t1.hand_mask);
        const int t2_num_cards = get_num_cards_in_hand_mask(t2.hand_mask);
        const bool t1_has_min_cards = t1_num_cards >= min_cards_for_turn;
        const bool t2_has_min_cards = t2_num_cards >= min_cards_for_turn;
        if (t1_has_min_cards != t2_has_min_cards)
        {
            // 1. Prefer minimum number of cards played.
            return t2_has_min_cards;
        }
        if (t1.delta != t2.delta)
        {
            // 2. Prefer smaller delta.
            return t2.delta < t1.delta;
        }
        else if (t1.reached_for_group != t2.reached_for_group)
        {
            // 3. Prefer the plays that didn't reach for a group.
            return t1.reached_for_group;
        }
        else if (t1_num_cards != t2_num_cards)
        {
            // 4. Prefer the turn that used more cards.
            return t2_num_cards > t1_num_cards;
        }
        else
        {
            // 5. Prefer to keep the numbers on the extreme intact.
            const int sum_of_pile_extremes1 = get_sum_of_pile_extremes(t1.piles);
            const int sum_of_pile_extremes2 = get_sum_of_pile_extremes(t2.piles);
            if (sum_of_pile_extremes2 < sum_of_pile_extremes1)
            {
                return true;
            }
        }

        return false;
    }

    Turn get_best_min_cards_all_piles(const Piles &piles, const PilesOfPlays &piles_of_plays, int min_cards_for_turn)
    {
        Turn t;
        t.piles = piles;
        std::optional<size_t> pi;
        while ((pi = get_next_min_play_piles_index(piles_of_plays, t.piles_indexes, t.hand_mask)).has_value())
        {
            const Play &play = piles_of_plays[*pi][t.piles_indexes[*pi]];
            update_turn_from_play(play, *pi, t);
            if (get_num_cards_in_hand_mask(t.hand_mask) >= min_cards_for_turn)
            {
                break;
            }
        }
        return t;
    }

    Turn get_best_min_cards_in_pile(const Piles &piles, const Plays &plays, size_t piles_index, int min_cards_for_turn)
    {
        Turn t;
        t.piles = piles;
        for (const auto &play : plays)
        {
            update_turn_from_play(play, piles_index, t);
            if (get_num_cards_in_hand_mask(t.hand_mask) >= min_cards_for_turn)
            {
                break;
            }
        }
        return t;
    }

    void play_reach_cards(const PilesOfPlays &piles_of_plays, int card_reach_distance, Turn &t)
    {
        std::optional<size_t> pi;
        while ((pi = get_next_min_play_piles_index(piles_of_plays, t.piles_indexes, t.hand_mask)).has_value())
        {
            const Play &play = piles_of_plays[*pi][t.piles_indexes[*pi]];
            if (play.delta > card_reach_distance)
            {
                break;
            }
            update_turn_from_play(play, *pi, t);
        }
    }

    Turn find_best_turn(const Piles &piles, const Hand &hand, int min_cards_for_turn, int card_reach_distance)
    {
        PilesOfPlays piles_of_plays = get_piles_of_plays(piles, hand, min_cards_for_turn, card_reach_distance);
        Turn best_turn = get_best_min_cards_all_piles(piles, piles_of_plays, min_cards_for_turn);
        for (size_t pi = 0; pi < piles.size(); ++pi)
        {
            auto pile_turn = get_best_min_cards_in_pile(piles, piles_of_plays[pi], pi, min_cards_for_turn);
            const TurnCompare turn_compare{min_cards_for_turn};
            if (turn_compare(best_turn, pile_turn))
            {
                best_turn = pile_turn;
            }
        }
        play_reach_cards(piles_of_plays, card_reach_distance, best_turn);
        return best_turn;
    }
} // namespace TheGameAnalyzer