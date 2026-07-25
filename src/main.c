#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card.h"
#include "deck.h"
#include "player.h"
#include "table.h"
#include "hand_eval.h"

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