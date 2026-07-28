#include "deck.h"

#include <stdio.h>
#include "player.h"
#include "table.h"

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

void shuffle_deck(Deck* deck) {
    for (size_t i = deck->cards_count - 1; i > 0; i--) {
        size_t random_index = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[random_index];
        deck->cards[random_index] = temp;
    }
}

int set_player_hand(Player* player, Deck* deck, const char* card1, const char* card2) {
    Card c1, c2;
    if (!parse_card(card1, &c1) || !parse_card(card2, &c2)) {
        fprintf(stderr, "Invalid card notation: \"%s\" or \"%s\"\n", card1, card2);
        return 0;
    }

    if (c1.value == c2.value && c1.suit == c2.suit) {
        fprintf(stderr, "Duplicate card: %s and %s are the same card\n", card1, card2);
        return 0;
    }

    if (!remove_card(deck, c1)) {
        fprintf(stderr, "Card %s is not available (already assigned elsewhere)\n", card1);
        return 0;
    }

    if (!remove_card(deck, c2)) {
        fprintf(stderr, "Card %s is not available (already assigned elsewhere)\n", card2);
        deck->cards[deck->cards_count] = c1;   // roll back c1 so deck stays consistent
        deck->cards_count++;
        return 0;
    }

    player->hand[0] = c1;
    player->hand[1] = c2;
    player->hand_count = 2;
    return 1;
}

Card get_card(Deck* deck) {
    return deck->cards[(deck->cards_count--) - 1];
}

void deal_card(Deck* deck, Player* player) {
    if (player->hand_count >= 2 || deck->cards_count == 0) return;

    Card card = get_card(deck);
    player->hand[player->hand_count++] = card;
}

void burn_card(Deck* deck, Table* table) {
    if (table->burns_count >= 3 || deck->cards_count == 0) return;

    Card card = get_card(deck);
    table->burns[table->burns_count++] = card;
}

int remove_card(Deck* deck, Card card) {
    for (size_t i = 0; i < deck->cards_count; i++) {
        if (deck->cards[i].value == card.value && deck->cards[i].suit == card.suit) {
            deck->cards[i] = deck->cards[deck->cards_count - 1];
            deck->cards_count--;
            return 1;
        }
    }
    return 0;
}

void deal_player_cards(Player* players, size_t num_players, Deck* deck) {
    for (int i = 0; i < 2; i++) {
        for (size_t j = 0; j < num_players; j++) {
            deal_card(deck, &players[j]);
        }
    }
}

void deal_flop(Deck* deck, Table* table) {
    if (table->board_count != 0) return;

    for (int i = 0; i < 3; i++) {
        Card card = get_card(deck);
        table->board[i] = card;
    }

    table->board_count = 3;
}

void deal_turn(Deck* deck, Table* table) {
    if (table->board_count != 3) return;

    Card card = get_card(deck);
    table->board[3] = card;
    table->board_count = 4;
}

void deal_river(Deck* deck, Table* table) {
    if (table->board_count != 4) return;

    Card card = get_card(deck);
    table->board[4] = card;
    table->board_count = 5;
}

void return_card(Deck* deck, Card card) {
    if (deck->cards_count >= 52) return;

    deck->cards[deck->cards_count++] = card;
}

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