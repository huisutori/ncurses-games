/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "mine_sweeper_settings.h"

MineSweeperSettings mine_sweeper_settings[MINE_SWEEPER_LEVEL_MAX] = {
    [MINE_SWEEPER_LEVEL_EASY] = {
        .num_rows = 9,
        .num_cols = 8,
        .num_bombs = 10,
    },
    [MINE_SWEEPER_LEVEL_MEDIUM] = {
        .num_rows = 10,
        .num_cols = 12,
        .num_bombs = 24,
    },
    [MINE_SWEEPER_LEVEL_HARD] = {
        .num_rows = 11,
        .num_cols = 15,
        .num_bombs = 36,
    },
};