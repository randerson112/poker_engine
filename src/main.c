#include <stdio.h>
#include <stdlib.h>

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

const char* value_to_string(Value v) {
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
    }
}

typedef enum {
    SPADES,
    DIAMONDS,
    CLUBS,
    HEARTS
} Suit;

const char* suit_to_string(Suit s) {
    switch (s) {
        case SPADES: return "♠️";
        case DIAMONDS: return "♦️";
        case CLUBS: return "♣️";
        case HEARTS: return "♥️";
    }
}

typedef struct {
    Value value;
    Suit suit;
} Card;

typedef struct {
    Card cards[52];
    size_t count;
} Deck;

typedef struct {
    int id;
    Card hand[2];
} Player;

Deck* create_deck() {
    Deck* deck = malloc(sizeof(Deck));
    int i = 0;

    for (int s = 0; s < 4; s++) {
        for (int v = 0; v < 13; v++) {
            Card card = {(Value)v, (Suit)s};
            deck->cards[i] = card;
            i++;
        }
    }

    return deck;
}

int main() {

    Deck* deck = create_deck();

    for (int i = 0; i < 52; i++) {
        const char* value = value_to_string(deck->cards[i].value);
        const char* suit = suit_to_string(deck->cards[i].suit);
        printf("%s%s\n", value, suit);
    }

    return 0;
}