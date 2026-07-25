#include "player.h"

// Creates the given number of players and returns a pointer to the array
Player* create_players(size_t num_players) {
    Player* players = malloc(sizeof(Player) * num_players);

    for (size_t i = 0; i < num_players; i++) {
        players[i].hand_count = 0;
    }

    return players;
}