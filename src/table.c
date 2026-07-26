#include "table.h"

#include <stdio.h>
#include <stdlib.h>

void print_board(Table* table) {
    for (size_t i = 0; i < table->board_count; i++) {
        printf("%s%s ", value_to_string(table->board[i].value), suit_to_string(table->board[i].suit));
    }
    printf("\n");
}