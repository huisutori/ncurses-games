/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "model/mine-sweeper/mine_cell.h"

typedef struct MineMapView {
    void (*on_draw)(struct MineMapView *view, size_t num_rows, size_t num_cols);
    void (*on_erase)(struct MineMapView *view);
    void (*on_update)(struct MineMapView *view, const MineCell *cell);
    void (*on_finish)(struct MineMapView *view, bool is_success);
    const MineCell *(*get_positioned_cell)(struct MineMapView *view, 
                                           size_t x, size_t y);
} MineMapView;

void MineMapView_on_draw(MineMapView *view, size_t num_rows, size_t num_cols);
void MineMapView_on_erase(MineMapView *view);
void MineMapView_on_update(MineMapView *view, const MineCell *cell);
void MineMapView_on_finish(MineMapView *view, bool is_success);

const MineCell *MineMapView_get_positioned_cell(MineMapView *view, 
                                                size_t x, size_t y);