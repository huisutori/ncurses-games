/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct MenuItemView {
    struct MenuItemView *up, *down, *left, *right;
    void (*on_draw)(struct MenuItemView *view);
    void (*on_erase)(struct MenuItemView *view);
    void (*on_focus)(struct MenuItemView *view);
    void (*on_release)(struct MenuItemView *view);
    void (*on_click)(struct MenuItemView *view);
    bool (*has_position)(struct MenuItemView *view, size_t x, size_t y);
} MenuItemView;

void MenuItemView_on_draw(MenuItemView *view);
void MenuItemView_on_erase(MenuItemView *view);
void MenuItemView_on_focus(MenuItemView *view);
void MenuItemView_on_release(MenuItemView *view);
void MenuItemView_on_click(MenuItemView *view);
bool MenuItemView_has_position(MenuItemView *view, size_t x, size_t y);