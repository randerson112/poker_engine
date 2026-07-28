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

// Converts a card value enum into a string.
const char* value_to_string(Value v);

// Converts a card suit enum into a string.
const char* suit_to_string(Suit s);

// Parses two-character card notation (e.g. "AS", "Td", "9h") into a Card.
// Returns 1 on success or 0 if the string is malformed.
int parse_card(const char* str, Card* out);

#endif // CARD_H