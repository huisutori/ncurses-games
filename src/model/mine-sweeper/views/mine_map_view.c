/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "mine_map_view.h"

void MineMapView_on_draw(MineMapView *view, size_t num_rows, size_t num_cols)
{
    if (view && view->on_draw) {
        view->on_draw(view, num_rows, num_cols);
    }
}

void MineMapView_on_erase(MineMapView *view)
{
    if (view && view->on_erase) {
        view->on_erase(view);
    }
}

void MineMapView_on_update(MineMapView *view, const MineCell *cell)
{
    if (view && view->on_update) {
        view->on_update(view, cell);
    }
}

void MineMapView_on_finish(MineMapView *view, bool is_success)
{
    if (view && view->on_finish) {
        view->on_finish(view, is_success);
    }
}

const MineCell *MineMapView_get_positioned_cell(MineMapView *view, 
                                                size_t x, size_t y)
{
    if (view && view->get_positioned_cell) {
        return view->get_positioned_cell(view, x, y);
    }
    return NULL;
}