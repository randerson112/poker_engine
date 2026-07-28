#include "equity.h"

#include <stdio.h>
#include "hand_eval.h"

void build_unkown_deck(Player* players, size_t num_players, Table* table, Deck* out_unknown) {
    int used[13][4] = {{0}};

    // Cards used by players
    for (size_t i = 0; i < num_players; i++) {
        for (size_t j = 0; j < players[i].hand_count; j++) {
            used[players[i].hand[j].value][players[i].hand[j].suit] = 1;
        }
    }

    // Cards used by board
    for (size_t i = 0; i < table->board_count; i++) {
        used[table->board[i].value][table->board[i].suit] = 1;
    }

    // Build deck excluding the used cards
    out_unknown->cards_count = 0;
    for (int s = 0; s < 4; s++) {
        for (int v = 0; v < 13; v++) {
            if (!used[v][s]) {
                out_unknown->cards[out_unknown->cards_count++] = (Card){(Value)v, (Suit)s};
            }
        }
    }
}

void calculate_all_equity(Player* players, size_t num_players, Table* table, int num_trials, Equity* out_equites) {
    Deck unknown = {0};
    build_unkown_deck(players, num_players, table, &unknown);

    int cards_left = 5 - (int)table->board_count;
    int trials = (cards_left == 0) ? 1 : num_trials;

    int wins[MAX_PLAYERS] = {0};
    int ties[MAX_PLAYERS] = {0};

    for (int t = 0; t < trials; t++) {
        if (cards_left > 0) shuffle_deck(&unknown);

        Table trial_table = *table;
        for (int i = 0; i < cards_left; i++) {
            trial_table.board[trial_table.board_count++] = unknown.cards[unknown.cards_count - 1 - i];
        }

        int winner_indices[MAX_PLAYERS];
        size_t num_winners = 0;
        HandScore best;
        determine_winners(players, num_players, &trial_table, winner_indices, &num_winners, &best);

        for (size_t w = 0; w < num_winners; w++) {
            int p = winner_indices[w];
            if (num_winners == 1) wins[p]++; else ties[p]++;
        }
    }

    for (size_t p = 0; p < num_players; p++) {
        out_equites[p].win_pct = 100.0 * wins[p] / trials;
        out_equites[p].tie_pct = 100.0 * ties[p] / trials;
        out_equites[p].lose_pct = 100.0 - out_equites[p].win_pct - out_equites[p].tie_pct;
    }
}

void print_equites(const char* street, Player* players, size_t num_players, Equity* equities) {
    printf("--%s Equities--\n\n", street);

    for (size_t p = 0; p < num_players; p++) {
        printf("Player %d:\n", (int)p + 1);
        printf("Win: %.2lf%%\n", equities[p].win_pct);
        printf("Tie: %.2lf%%\n", equities[p].tie_pct);
        printf("Lose: %.2lf%%\n", equities[p].lose_pct);
        if (p != (int)num_players - 1) printf("\n");
    }
}
