#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include "card.h"

typedef struct {
    Card board[5];
    size_t board_count;
    Card burns[3];
    size_t burns_count;
    Card muck[52];
    size_t muck_count;
} Table;

#endif