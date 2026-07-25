#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "card.h"

typedef struct {
    Card hand[2];
    size_t hand_count;
} Player;

// Creates the given number of players and returns a pointer to the array.
Player* create_players(size_t num_players);

#endif