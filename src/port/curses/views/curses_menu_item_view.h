/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <ncurses.h>
#include "model/menu/menu.h"
#include "model/menu/views/menu_item_view.h"

MenuItemView *CursesMenuItemView_new(const MenuItem *item);
void CursesMenuItemView_delete(MenuItemView *view);

void CursesMenuItemView_set_window(MenuItemView *view, WINDOW *window);
WINDOW *CursesMenuItemView_get_window(MenuItemView *view);

#define CURSES_MENU_ITEM_VIEW_COLOR         SCREEN_COLOR_DARK_BLUE
#define CURSES_MENU_ITEM_VIEW_FOCUSED_COLOR SCREEN_COLOR_BLUE
#define CURSES_MENU_ITEM_VIEW_CONTENT_COLOR SCREEN_COLOR_YELLOW