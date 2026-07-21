#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
        default: return "?";
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
        default: return "?";
    }
}

typedef struct {
    Value value;
    Suit suit;
} Card;

typedef struct {
    Card cards[52];
    size_t cards_count;
} Deck;

typedef struct {
    Card hand[2];
    size_t hand_count;
} Player;

typedef struct {
    Card board[5];
    size_t board_count;
    Card burns[3];
    size_t burns_count;
    Card muck[52];
    size_t muck_count;
} Table;

// Creates a new deck of 52 cards
Deck* create_deck() {
    Deck* deck = malloc(sizeof(Deck));
    deck->cards_count = 52;

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

// Shuffles the deck using Fisher-Yates shuffle algorithm
void shuffle_deck(Deck* deck) {
    for (size_t i = deck->cards_count - 1; i > 0; i--) {
        size_t random_index = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[random_index];
        deck->cards[random_index] = temp;
    }
}

// Deals a single card to a player
void deal_card(Deck* deck, Player* player) {
    if (player->hand_count >= 2 || deck->cards_count == 0) return;

    Card card = deck->cards[(deck->cards_count--) - 1];
    player->hand[player->hand_count++] = card;
}

// Returns a card to the deck
void return_card(Deck* deck, Card card) {
    if (deck->cards_count >= 52) return;

    deck->cards[deck->cards_count++] = card;
}

// Returns all used cards to the deck
void clean_up_hand(Player* players, size_t num_players, Table* table, Deck* deck) {

    // Player cards
    for (size_t i = 0; i < num_players; i++) {
        for (size_t j = 0; j < players[i].hand_count; j++) {
            return_card(deck, players[i].hand[j]);
        }
        players[i].hand_count = 0;
    }

    // Board cards
    for (size_t i = 0; i < table->board_count; i++) {
        return_card(deck, table->board[i]);
    }
    table->board_count = 0;

    // Burn cards
    for (size_t i = 0; i < table->burns_count; i++) {
        return_card(deck, table->burns[i]);
    }
    table->burns_count = 0;

    // Muck cards
    for (size_t i = 0; i < table->muck_count; i++) {
            return_card(deck, table->muck[i]);
        }
        table->muck_count = 0;
}

int main() {

    srand(time(NULL));

    Deck* deck = create_deck();
    shuffle_deck(deck);

    for (int i = 0; i < 52; i++) {
        const char* value = value_to_string(deck->cards[i].value);
        const char* suit = suit_to_string(deck->cards[i].suit);
        printf("%s%s\n", value, suit);
    }

    return 0;
}