/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MINE_CELL_UNKNOWN,
    MINE_CELL_OPENED,
    MINE_CELL_FLAGGED,
} MineCellState;

typedef struct {
    MineCellState state;
    uint8_t bomb_count;
    bool is_bomb;
} MineCell;