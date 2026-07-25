#ifndef HAND_EVAL_H
#define HAND_EVAL_H

#include "card.h"

typedef enum {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRIGHT_FLUSH
} HandRank;

typedef struct {
    HandRank rank;
    Value tiebreakers[5];
} HandScore;

static const char* rank_to_string(HandRank r) {
    switch (r) {
        case HIGH_CARD: return "High Card";
        case PAIR: return "Pair";
        case TWO_PAIR: return "Two Pair";
        case THREE_OF_A_KIND: return "Three of a Kind";
        case STRAIGHT: return "Straight";
        case FLUSH: return "Flush";
        case FULL_HOUSE: return "Full House";
        case FOUR_OF_A_KIND: return "Four of a Kind";
        case STRIGHT_FLUSH: return "Straight Flush";
        default: return "?";
    }
}

static inline const char* hand_description(HandScore score) {
    if (score.rank == STRIGHT_FLUSH && score.tiebreakers[0] == ACE) {
        return "Royal Flush";
    }
    return rank_to_string(score.rank);
}

// Evaluates the best 5 card hand from 7 cards.
// Keeps track of needed tie-breaking information.
HandScore evaluate_best_hand(Card cards[7]);

// Compares two hands to see which is better.
// Positive: a is better
// Negative: b is better
// Zero: Hands are the same
int compare_scores(HandScore a, HandScore b);

#endif // HAND_EVAL_H