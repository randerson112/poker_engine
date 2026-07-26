#include "player.h"

#include <stdio.h>

Player* create_players(size_t num_players) {
    Player* players = malloc(sizeof(Player) * num_players);

    for (size_t i = 0; i < num_players; i++) {
        players[i].hand_count = 0;
    }

    return players;
}

void print_players(Player* players, size_t num_players) {
    for (size_t i = 0; i < num_players; i++) {
        Card card1 = players[i].hand[0];
        Card card2 = players[i].hand[1];
        printf("Player %d: %s%s %s%s\n", (int)i + 1, value_to_string(card1.value), suit_to_string(card1.suit), value_to_string(card2.value), suit_to_string(card2.suit));
    }
}