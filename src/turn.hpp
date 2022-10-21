#pragma once

#include <array>
#include <string>
#include <vector>

namespace TheGameAnalyzer
{
    using Card = int16_t;
    inline void flip_card(Card &c) { c = 101 - c; }

    using Hand = std::vector<Card>;

    std::string to_string(const Hand &hand);

    // Flip the cards and reverse the order.
    void flip_hand(Hand &hand);

    // Bit mask for Hand. (Note max of 8 cards in a hand.)
    using HandMask = uint16_t;

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
        inline bool is_group_reach() const { return delta > 0 && get_num_cards_in_hand_mask(hand_mask) > 1; }
    };
    bool operator==(const Play &p1, const Play &p2);
    bool operator!=(const Play &p1, const Play &p2);
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
        size_t lo{0}; // hand index for first of ten group.
        size_t hi{0}; // hand index for last of ten group.
        HandMask hand_mask{0};
    };
    bool operator==(const TenGroup &tg1, const TenGroup &tg2);
    bool operator!=(const TenGroup &tg1, const TenGroup &tg2);
    std::string to_string(const TenGroup &);

    // Container for 10s groupings.
    struct TenGroups
    {
        std::vector<TenGroup> groups;
        HandMask groups_hand_mask{0};
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

    // Find plays for this pile.
    //
    // Finds for an ascending pile. For descending piles, flip them first.
    //
    // \param pile_card Top card of pile in consideration.
    // \param max_card Don't consider cards >= max_card for plays.
    // \param piles_index Index of piles.
    // \param ten_groups Collection of ten_groups in the hand.
    // \param min_cards_for_turn Minimum cards to be played for this turn.
    // \param card_reach_distance Amount to "reach" to play another card.
    // \return Possible plays for this pile.
    Plays get_plays_ascending(Card pile_card, Card max_card, size_t piles_index,
                              const Hand &hand, const TenGroups &ten_groups,
                              int min_cards_for_turn, int card_reach_distance);

    // Plays index for each pile of plays.
    using PilesIndexes = std::array<size_t, 4>;

    std::string to_string(PilesIndexes);

    // The intermediate state and outcome of a player turn.
    struct Turn
    {
        Piles piles{0};
        HandMask hand_mask{0};
        int delta{0};
        PilesIndexes piles_indexes{0};
        bool reached_for_group{false};
    };
    bool operator==(const Turn &t1, const Turn &t2);
    bool operator!=(const Turn &t1, const Turn &t2);
    std::string to_string(const Turn &t);

    struct TurnCompare
    {
        int min_cards_for_turn;
        // \return true if t2 is better than t1.
        bool operator()(const Turn &t1, const Turn &t2) const;
    };

    // Compare for starting hands
    bool starting_hand_turn_compare(const Turn &t1, const Turn &t2);

    // Find the best turn for a given set of piles and hand.
    //
    // \param piles Game piles.
    // \param hand Hand.
    // \param min_cards_for_turn Minimum cards to be played for this turn.
    // \param card_reach_distance Amount to "reach" to play another card.
    Turn find_best_turn(const Piles &, const Hand &,
                        int min_cards_for_turn,
                        int card_reach_distance);

} // namespace TheGameAnalyzer
