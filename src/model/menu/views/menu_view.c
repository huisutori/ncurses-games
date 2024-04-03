/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "menu_view.h"

void MenuView_on_draw(MenuView *view)
{
    if (view && view->on_draw) {
        view->on_draw(view);
    }
}

void MenuView_on_erase(MenuView *view)
{
    if (view && view->on_erase) {
        view->on_erase(view);
    }
}

void MenuView_on_screen_resize(MenuView *view, size_t width, size_t height)
{
    if (view && view->on_screen_resize) {
        view->on_screen_resize(view, width, height);
    }
}

MenuItemView *MenuView_get_first_item(MenuView *view)
{
    if (view && view->get_first_item) {
        return view->get_first_item(view);
    }
    return NULL;
}

MenuItemView *MenuView_get_positioned_item(MenuView *view, size_t x, size_t y)
{
    if (view && view->get_positioned_item) {
        return view->get_positioned_item(view, x, y);
    }
    return NULL;
}