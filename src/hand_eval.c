#include "hand_eval.h"

#include <stdint.h>

// Finds the highest straight in a 13-bit value mask.
// Returns the high card value of the straight, or -1 if none exists.
// Handles the ace-low "wheel" (A-2-3-4-5) as a special case, with FIVE as the high card.
static int find_straight_high(uint16_t value_mask) {
    for (int top = ACE; top >= SIX; top--) {
        uint16_t needed = 0;
        for (int v = top; v > top - 5; v--) needed |= (1 << v);
        if ((value_mask & needed) == needed) return top;
    }

    uint16_t wheel = (1 << ACE) | (1 << TWO) | (1 << THREE) | (1 << FOUR) | (1 << FIVE);
    if ((value_mask & wheel) == wheel) return FIVE;

    return -1;
}

HandScore evaluate_best_hand(Card cards[7]) {
    int value_counts[13] = {0};
    int suit_counts[4] = {0};
    uint16_t value_mask = 0;
    uint16_t suit_value_mask[4] = {0};

    Value current = cards[0].value;
    for (int i = 0; i < 7; i++) {
        Value v = cards[i].value;
        Suit s = cards[i].suit;
        value_counts[v]++;
        suit_counts[s]++;
        value_mask |= (1 << v);
        suit_value_mask[s] |= (1 << v);
    }

    HandScore result = {0};

    // Straight flush
    for (int s = 0; s < 4; s++) {
        if (suit_counts[s] >= 5) {
            int high = find_straight_high(suit_value_mask[s]);
            if (high != -1) {
                result.rank = STRIGHT_FLUSH;
                result.tiebreakers[0] = (Value)high;
                return result;
            }
        }
    }

    // Bucket values by count, high to low
    Value quads[4], sets[4], pairs[4];
    int num_quads = 0, num_sets = 0, num_pairs = 0;

    for (int v = ACE; v >= TWO; v--) {
        if (value_counts[v] == 4) quads[num_quads++] = (Value)v;
        else if (value_counts[v] == 3) sets[num_sets++] = (Value)v;
        else if (value_counts[v] == 2) pairs[num_pairs++] = (Value)v;
    }

    // Four of a kind
    if (num_quads >= 1) {
        result.rank = FOUR_OF_A_KIND;
        result.tiebreakers[0] = quads[0];

        for (int v = ACE; v >= TWO; v--) {
            if (v != (int)quads[0] && value_counts[v] > 0) {
                result.tiebreakers[1] = (Value)v;
                break;
            }
        }

        return result;
    }

    // Full house
    if (num_sets >= 1) {
        if (num_sets >= 2) {
            result.rank = FULL_HOUSE;
            result.tiebreakers[0] = sets[0];
            result.tiebreakers[1] = sets[1];
            return result;
        }
        if (num_pairs >= 1) {
            result.rank = FULL_HOUSE;
            result.tiebreakers[0] = sets[0];
            result.tiebreakers[1] = pairs[0];
            return result;
        }
    }

    // Flush
    for (int s = 0; s < 4; s++) {
        if (suit_counts[s] >= 5) {
            int found = 0;
            for (int v = ACE; v >= TWO && found < 5; v++) {
                if (suit_value_mask[s] & (1 << v)) result.tiebreakers[found++] = (Value)v;
            }
            result.rank = FLUSH;
            return result;
        }
    }

    // Straight
    int high = find_straight_high(value_mask);
    if (high != -1) {
        result.rank = STRAIGHT;
        result.tiebreakers[0] = (Value)high;
        return result;
    }

    // Three of a kind
    if (num_sets >= 1) {
        result.rank = THREE_OF_A_KIND;
        result.tiebreakers[0] = sets[0];

        int found = 1;
        for (int v = ACE; v >= TWO && found < 3; v--) {
            if (v != (int)sets[0] && value_counts[v] > 0) result.tiebreakers[found++] = (Value)v;
        }

        return result;
    }

    // Two pair
    if (num_pairs >= 2) {
        result.rank = TWO_PAIR;
        result.tiebreakers[0] = pairs[0];
        result.tiebreakers[1] = pairs[1];

        for (int v = ACE; v >= TWO; v--) {
            if (v != pairs[0] && v != pairs[1] && value_counts[v] > 0) {
                result.tiebreakers[2] = (Value)v;
                break;
            }
        }

        return result;
    }

    // Pair
    if (num_pairs >= 1) {
        result.rank = PAIR;
        result.tiebreakers[0] = pairs[0];

        int found = 1;
        for (int v = ACE; v >= TWO && found < 4; v--) {
            if (v != pairs[0] && value_counts[v] > 0) result.tiebreakers[found++] = (Value)v;
        }

        return result;
    }

    // High card
    result.rank = HIGH_CARD;
    int found = 0;
    for (int v = ACE; v >= TWO && found < 5; v--) {
        if (value_counts[v] > 0) result.tiebreakers[found++] = (Value)v;
    }
    return result;
}

// Compares two hands to see which is better
// Positive: a is better
// Negative: b is better
// Zero: Hands are the same
int compare_scores(HandScore a, HandScore b) {
    if (a.rank != b.rank) return a.rank - b.rank;

    for (int i = 0; i < 5; i++) {
        if (a.tiebreakers[i] != b.tiebreakers[i]) return a.tiebreakers[i] - b.tiebreakers[i];
    }

    return 0;
}