/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include "model/menu/menu.h"
#include "model/mine-sweeper/views/mine_sweeper_view.h"

MineSweeperView *CursesMineSweeperView_new(size_t num_rows, size_t num_cols, 
                                           size_t num_menu_items, 
                                           MenuItem menu_items[]);
void CursesMineSweeperView_delete(MineSweeperView *super);