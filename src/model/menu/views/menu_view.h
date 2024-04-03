/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include "menu_item_view.h"

typedef struct MenuView {
    void (*on_draw)(struct MenuView *view);
    void (*on_erase)(struct MenuView *view);
    void (*on_screen_resize)(struct MenuView *view, size_t width, size_t height);
    MenuItemView *(*get_first_item)(struct MenuView *view);
    MenuItemView *(*get_positioned_item)(struct MenuView *view, size_t x, size_t y);
} MenuView;

void MenuView_on_draw(MenuView *view);
void MenuView_on_erase(MenuView *view);
void MenuView_on_screen_resize(MenuView *view, size_t width, size_t height);

MenuItemView *MenuView_get_first_item(MenuView *view);
MenuItemView *MenuView_get_positioned_item(MenuView *view, size_t x, size_t y);