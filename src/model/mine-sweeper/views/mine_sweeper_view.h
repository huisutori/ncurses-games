/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stdint.h>
#include "model/menu/views/menu_item_view.h"
#include "mine_map_view.h"

typedef struct MineSweeperView {
    void (*on_draw)(struct MineSweeperView *view);
    void (*on_erase)(struct MineSweeperView *view);
    void (*on_timer_update)(struct MineSweeperView *view, uint32_t time);
    void (*on_bomb_count_update)(struct MineSweeperView *view, uint16_t bomb_count);
    MenuItemView *(*get_first_menu_item)(struct MineSweeperView *view);
    MenuItemView *(*get_positioned_menu_item)(struct MineSweeperView *view, 
                                         size_t x, size_t y);
    MineMapView *(*get_map_view)(struct MineSweeperView *view);
} MineSweeperView;

void MineSweeperView_on_draw(MineSweeperView *view);
void MineSweeperView_on_erase(MineSweeperView *view);

void MineSweeperView_on_timer_update(MineSweeperView *view, uint32_t time);
void MineSweeperView_on_bomb_count_update(MineSweeperView *view, uint16_t bomb_count);

MenuItemView *MineSweeperView_get_first_menu_item(MineSweeperView *view);
MenuItemView *MineSweeperView_get_positioned_menu_item(MineSweeperView *view, 
                                                       size_t x, size_t y);
MineMapView *MineSweeperView_get_map_view(MineSweeperView *view);