/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    MINE_CELL_UNKNOWN,
    MINE_CELL_OPENED,
    MINE_CELL_FLAGGED,
    MINE_CELL_ERROR,
} MineCellState;

typedef struct {
    MineCellState state;
    size_t x, y;
    size_t bomb_count;
    bool is_bomb;
} MineCell;