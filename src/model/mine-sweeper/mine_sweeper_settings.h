/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>

typedef enum {
    MINE_SWEEPER_LEVEL_EASY,
    MINE_SWEEPER_LEVEL_MEDIUM,
    MINE_SWEEPER_LEVEL_HARD,
    MINE_SWEEPER_LEVEL_MAX,
} MineSweeperLevel;

typedef struct {
    size_t num_rows;
    size_t num_cols;
    size_t num_bombs;
} MineSweeperSettings;

extern MineSweeperSettings mine_sweeper_settings[MINE_SWEEPER_LEVEL_MAX];