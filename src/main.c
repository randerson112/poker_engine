#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card.h"
#include "hand_eval.h"

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

// Creates the given number of players and returns a pointer to the array
Player* create_players(size_t num_players) {
    Player* players = malloc(sizeof(Player) * num_players);

    for (size_t i = 0; i < num_players; i++) {
        players[i].hand_count = 0;
    }

    return players;
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

// Removes and returns the top card of the deck
Card get_card(Deck* deck) {
    return deck->cards[(deck->cards_count--) - 1];
}

// Deals a single card to a player
void deal_card(Deck* deck, Player* player) {
    if (player->hand_count >= 2 || deck->cards_count == 0) return;

    Card card = get_card(deck);
    player->hand[player->hand_count++] = card;
}

// Burns the top card of the deck
void burn_card(Deck* deck, Table* table) {
    if (table->burns_count >= 3 || deck->cards_count == 0) return;

    Card card = get_card(deck);
    table->burns[table->burns_count++] = card;
}

// Deals two cards to each player
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

// Determines the winners of a hand.
// Returns the indices in the players list and the number of winners to the out values.
// Multiple winners means a split pot.
void determine_winners(Player* players, size_t num_players, Table* table, int* out_winner_indices, size_t* out_num_winners, HandScore* out_best_score) {
    HandScore best_score = {0};
    *out_num_winners = 0;

    for (size_t i = 0; i < num_players; i++) {
        Card seven[7];
        seven[0] = players[i].hand[0];
        seven[1] = players[i].hand[1];
        for (size_t b = 0; b < 5; b++) {
            seven[2 + b] = table->board[b];
        }

        HandScore score = evaluate_best_hand(seven);
        int compare = compare_scores(score, best_score);

        // Check if new best hand is found
        if (*out_num_winners == 0 || compare > 0) {
            best_score = score;
            out_winner_indices[0] = (int)i;
            *out_num_winners = 1;
        } else if (compare == 0) {
            out_winner_indices[(*out_num_winners)++] = (int)i; // Ties (split pot)
        }
    }

    *out_best_score = best_score;
}

int main() {

    srand(time(NULL));

    size_t num_players = 4;
    Player* players = create_players(num_players);

    Deck* deck = create_deck();
    shuffle_deck(deck);

    Table table = {0};

    deal_player_cards(players, num_players, deck);

    for (size_t i = 0; i < num_players; i++) {
        printf("Player %zu: %s%s %s%s\n", i + 1, value_to_string(players[i].hand[0].value), suit_to_string(players[i].hand[0].suit), value_to_string(players[i].hand[1].value), suit_to_string(players[i].hand[1].suit));
    }

    burn_card(deck, &table);
    deal_flop(deck, &table);

    burn_card(deck, &table);
    deal_turn(deck, &table);

    burn_card(deck, &table);
    deal_river(deck, &table);

    for (size_t i = 0; i < table.board_count; i++) {
        printf("%s%s ", value_to_string(table.board[i].value), suit_to_string(table.board[i].suit));
    }
    printf("\n");

    // Determine winners
    int winner_indices[4];
    size_t num_winners = 0;
    HandScore winning_hand;

    determine_winners(players, num_players, &table, winner_indices, &num_winners, &winning_hand);

    if (num_winners == 1) {
        printf("Winner: Player %d with %s\n", winner_indices[0] + 1, hand_description(winning_hand));
    } else {
        printf("Split pot (%s) between: ", hand_description(winning_hand));
        for (size_t i = 0; i < num_winners; i++) {
            printf("Player %d ", winner_indices[i] + 1);
        }
        printf("\n");
    }

    clean_up_hand(players, num_players, &table, deck);

    // printf("%zu\n", deck->cards_count);
    // for (int i = 0; i < 52; i++) {
    //     const char* value = value_to_string(deck->cards[i].value);
    //     const char* suit = suit_to_string(deck->cards[i].suit);
    //     printf("%s%s\n", value, suit);
    // }

    free(deck);
    free(players);

    return 0;
}