#ifndef EQUITY_H
#define EQUITY_H

#include <stdlib.h>
#include "deck.h"
#include "player.h"
#include "table.h"

typedef struct {
    double win_pct;
    double tie_pct;
    double lose_pct;
} Equity;

// Builds a deck of unused cards not in players' hands or on the board.
void build_unkown_deck(Player* players, size_t num_players, Table* table, Deck* out_unknown);

// Computes live win/tie/lose equity for every non-folded player in a single trial loop
void calculate_all_equity(Player* players, size_t num_players, Table* table, int num_trials, Equity* out_equites);

// Prints player equities at the current state of the hand
void print_equites(const char* street, Player* players, size_t num_players, Equity* equities);

#endif