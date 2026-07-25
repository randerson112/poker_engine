#ifndef CARD_H
#define CARD_H

typedef enum {
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
} Value;

typedef enum {
    SPADES,
    DIAMONDS,
    CLUBS,
    HEARTS
} Suit;

typedef struct {
    Value value;
    Suit suit;
} Card;

static inline const char* value_to_string(Value v) {
    switch (v) {
        case TWO: return "2";
        case THREE: return "3";
        case FOUR: return "4";
        case FIVE: return "5";
        case SIX: return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE: return "9";
        case TEN: return "T";
        case JACK: return "J";
        case QUEEN: return "Q";
        case KING: return "K";
        case ACE: return "A";
        default: return "?";
    }
}

static inline const char* suit_to_string(Suit s) {
    switch (s) {
        case SPADES: return "♠️";
        case DIAMONDS: return "♦️";
        case CLUBS: return "♣️";
        case HEARTS: return "♥️";
        default: return "?";
    }
}

#endif // CARD_H