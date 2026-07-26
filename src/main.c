#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card.h"
#include "deck.h"
#include "player.h"
#include "table.h"
#include "hand_eval.h"
#include "equity.h"

int main() {

    srand(time(NULL));

    // Create players
    size_t num_players = 4;
    Player* players = create_players(num_players);

    // Create 52 card deck
    Deck* deck = create_deck();
    shuffle_deck(deck);

    // Create table
    Table table = {0};

    // Deal cards to players
    deal_player_cards(players, num_players, deck);
    print_players(players, num_players);

    // Calculate preflop equities
    Equity equities[num_players];
    calculate_all_equity(players, num_players, &table, 50000, equities);
    print_equites("Preflop", players, num_players, equities);

    // Deal flop and recalculate equities
    burn_card(deck, &table);
    deal_flop(deck, &table);
    print_board(&table);
    calculate_all_equity(players, num_players, &table, 100000, equities);
    print_equites("Flop", players, num_players, equities); 

    // Deal turn and recalculate equities
    burn_card(deck, &table);
    deal_turn(deck, &table);
    print_board(&table);
    calculate_all_equity(players, num_players, &table, 100000, equities);
    print_equites("Turn", players, num_players, equities);

    // Deal river and recalculate equities
    burn_card(deck, &table);
    deal_river(deck, &table);
    print_board(&table);
    calculate_all_equity(players, num_players, &table, 100000, equities);
    print_equites("River", players, num_players, equities);

    // Determine winners
    int winner_indices[MAX_PLAYERS];
    size_t num_winners = 0;
    HandScore winning_hand;

    determine_winners(players, num_players, &table, winner_indices, &num_winners, &winning_hand);
    print_winners(winner_indices, num_winners, winning_hand);

    // Return all cards to the deck
    clean_up_hand(players, num_players, &table, deck);

    // Clean up memory
    free(deck);
    free(players);

    return 0;
}