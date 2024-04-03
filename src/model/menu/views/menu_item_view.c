/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "menu_item_view.h"

void MenuItemView_on_draw(MenuItemView *view)
{
    if (view && view->on_draw) {
        view->on_draw(view);
    }
}

void MenuItemView_on_erase(MenuItemView *view)
{
    if (view && view->on_erase) {
        view->on_erase(view);
    }
}

void MenuItemView_on_focus(MenuItemView *view)
{
    if (view && view->on_focus) {
        view->on_focus(view);
    }
}

void MenuItemView_on_release(MenuItemView *view)
{
    if (view && view->on_release) {
        view->on_release(view);
    }
}

void MenuItemView_on_click(MenuItemView *view)
{
    if (view && view->on_click) {
        view->on_click(view);
    }
}

bool MenuItemView_has_position(MenuItemView *view, size_t x, size_t y)
{
    if (view && view->has_position) {
        return view->has_position(view, x, y);
    }
    return false;
}