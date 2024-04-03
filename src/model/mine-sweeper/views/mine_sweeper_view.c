/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "mine_sweeper_view.h"

void MineSweeperView_on_draw(MineSweeperView *view)
{
    if (view && view->on_draw) {
        view->on_draw(view);
    }
}

void MineSweeperView_on_erase(MineSweeperView *view)
{
    if (view && view->on_erase) {
        view->on_erase(view);
    }
}

void MineSweeperView_on_timer_update(MineSweeperView *view, uint32_t time)
{
    if (view && view->on_timer_update) {
        view->on_timer_update(view, time);
    }
}

void MineSweeperView_on_bomb_count_update(MineSweeperView *view, uint16_t bomb_count)
{
    if (view && view->on_bomb_count_update) {
        view->on_bomb_count_update(view, bomb_count);
    }
}

MenuItemView *MineSweeperView_get_first_menu_item(MineSweeperView *view)
{
    if (view && view->get_first_menu_item) {
        return view->get_first_menu_item(view);
    }
    return NULL;
}

MenuItemView *MineSweeperView_get_positioned_menu_item(MineSweeperView *view, 
                                                       size_t x, size_t y)
{
    if (view && view->get_positioned_menu_item) {
        return view->get_positioned_menu_item(view, x, y);
    }
    return NULL;
}       
                                                
MineMapView *MineSweeperView_get_map_view(MineSweeperView *view)
{
    if (view->get_map_view) {
        return view->get_map_view(view);
    }
    return NULL;
}