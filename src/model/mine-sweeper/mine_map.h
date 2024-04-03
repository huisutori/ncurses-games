/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "mine_sweeper_settings.h"
#include "mine_cell.h"

typedef void (*MineMapUpdateHandler)(const MineCell *cell);
typedef void (*MineMapResultHandler)(bool is_success);

int MineMap_init(const MineSweeperSettings *settings,  
                 MineMapUpdateHandler on_update, 
                 MineMapResultHandler on_result);
void MineMap_deinit(void);

void MineMap_reset(void);

void MineMap_open_cell(const MineCell *cell);
void MineMap_flag_cell(const MineCell *cell);
void MineMap_unflag_cell(const MineCell *cell);

const MineCell *MineMap_get_cell(size_t x, size_t y);
void MineMap_get_size(size_t *num_rows, size_t *num_cols);