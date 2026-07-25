#ifndef DECK_H
#define DECK_H

#include <stdlib.h>
#include "card.h"
#include "table.h"
#include "player.h"

typedef struct {
    Card cards[52];
    size_t cards_count;
} Deck;

// Creates a new deck of 52 cards.
Deck* create_deck();

// Shuffles the deck using Fisher-Yates shuffle algorithm.
void shuffle_deck(Deck* deck);

// Removes and returns the top card of the deck.
Card get_card(Deck* deck);

// Deals a single card to a player.
void deal_card(Deck* deck, Player* player);

// Burns the top card of the deck.
void burn_card(Deck* deck, Table* table);

// Deals two cards to each player.
void deal_player_cards(Player* players, size_t num_players, Deck* deck);

// Deals the first 3 cards to the board.
void deal_flop(Deck* deck, Table* table);

// Deals the 4th card to the board.
void deal_turn(Deck* deck, Table* table);

// Deals the 5th card to the board.
void deal_river(Deck* deck, Table* table);

// Returns a card to the deck.
void return_card(Deck* deck, Card card);

// Returns all used cards to the deck.
void clean_up_hand(Player* players, size_t num_players, Table* table, Deck* deck);

#endif // DECK_H