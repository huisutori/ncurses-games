/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include "model/screen.h"
#include "model/menu/views/menu_view.h"

MenuView *CursesMenuView_new(const Menu *menu, size_t item_width, size_t item_height);
void CursesMenuView_delete(MenuView *view);

#define CURSES_MENU_VIEW_COLOR SCREEN_COLOR_BLUE