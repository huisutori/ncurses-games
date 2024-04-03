/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <ncurses.h>
#include "model/mine-sweeper/views/mine_map_view.h"

void CursesMineMapView_set_window(WINDOW *window);
WINDOW *CursesMineMapView_get_window(void);

extern MineMapView curses_mine_map_view;

#define CURSES_MINE_MAP_VIEW_GET_WIDTH(num_cols)    ((num_cols) * 2 + 3)
#define CURSES_MINE_MAP_VIEW_GET_HEIGHT(num_rows)   ((num_rows) + 2)