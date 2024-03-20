/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stdint.h>
#include "mine_cell.h"

typedef struct MineMap MineMap;

MineMap *MineMap_new(uint8_t num_rows, uint8_t num_cols, 
                     uint8_t num_bombs);
void MineMap_delete(MineMap *map);

bool MineMap_open_cell(MineMap *map, uint8_t x, uint8_t y);
void MineMap_flag_cell(MineMap *map, uint8_t x, uint8_t y);