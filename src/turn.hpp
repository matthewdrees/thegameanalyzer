#pragma once

#include <array>
#include <utility>
#include <vector>

namespace TheGameAnalyzer
{
    using Card = int16_t;
    void flip_card(Card &c) { c = 99 - c; }

    using Hand = std::vector<Card>;

    void to_string(const Hand &hand);

    // Flip the cards and reverse the order.
    void flip_hand(Hand &hand);

    // Bit mask for Hand. (Note max of 8 cards in a hand.)
    using HandMask = uint8_t;

    // Flip the bits in a hand.
    void flip_hand_mask(HandMask &hand_mask, size_t hand_size);

    // Get the number of cards in the hand mask (i.e. count the bits)
    int get_num_cards_in_hand_mask(HandMask hand_mask);

    // Play of one or more cards out of hand to a pile.
    struct Play
    {
        HandMask hand_mask{0};
        size_t piles_index{0};
        Card pile_card_start{0};
        Card pile_card_end{0};
        int delta{0};
    };
    bool operator==(const Play &m1, const Play &m2);
    bool operator!=(const Play &m1, const Play &m2);
    std::string to_string(const Play &);

    // Flip the play.
    void flip_play(Play &, size_t hand_size);

    using Plays = std::vector<Play>;
    std::string to_string(const Plays &);

    // Flip the plays
    void flip_plays(Plays &plays, size_t hand_size);

    // Hand index for ten grouping (could be more than 2 cards, for hand of
    // [ 5, 10, 20, 30, 40], indexes would be 1, 4).
    struct TenGroup
    {
        size_t lo{0}; // low hand index.
        size_t hi{0}; // high hand index.
        HandMask hand_mask{0};
    };
    bool operator==(const TenGroup &tg1, const TenGroup &tg2);
    bool operator!=(const TenGroup &tg1, const TenGroup &tg2);
    std::string to_string(const TenGroup &);

    // Container for 10s groupings.
    struct TenGroups
    {
        std::vector<TenGroup> groups;
        HandMask groups_hand_mask = 0;
        const TenGroup &operator[](size_t idx) const { return groups[idx]; };
        void push_back(TenGroup &tg) { groups.push_back(tg); };
        std::vector<TenGroup>::const_iterator begin() const { return groups.begin(); };
        std::vector<TenGroup>::const_iterator end() const { return groups.end(); };
    };
    bool operator==(const TenGroups &tg1, const TenGroups &tg2);
    bool operator!=(const TenGroups &tg1, const TenGroups &tg2);
    std::string to_string(const TenGroups &);

    // Get ten groupings for a given hand.
    TenGroups get_ten_groups(const Hand &hand);

    using Piles = std::array<Card, 4>;
    std::string to_string(const Piles &);

    // The outcome of a player turn.
    struct Turn
    {
        Turn() = default;
        explicit Turn(const Piles &piles_) : piles(piles_)
        {
        }
        Piles piles{1, 1, 100, 100};
        HandMask hand_mask{0};
    };

    // Used for finding the best turn. (Lower is better.)
    constexpr bool operator<(const Turn &t1, const Turn &t2);

    // Find moves for this pile.
    //
    // Finds for an ascending pile. For descending piles, flip them first.
    //
    // \param pile_card Top card of pile in consideration.
    // \param piles_index Index of pile.
    // \param ten_groups Collection of ten_groups in the hand.
    // \param min_cards_for_turn Minimum cards to be played for this turn.
    // \param card_reach_distance Amount to "reach" to play another card.
    // \return Possible plays for this pile.
    Plays get_plays_ascending(Card pile_card, size_t piles_index, const Hand &hand,
                              const TenGroups &ten_groups,
                              int min_cards_for_turn, int card_reach_distance);

    // Find the best turn for a given set of piles and hand.
    // \param pile_card Top card of pile in consideration.
    // \param ten_groups Collection of ten_groups in the hand.
    // \param min_cards_for_turn Minimum cards to be played for this turn.
    // \param card_reach_distance Amount to "reach" to play another card.
    Turn find_best_turn(const Piles &, Hand &, int min_cards_for_turn,
                        int card_reach_distance);

} // namespace TheGameAnalyzer
