/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <stdbool.h>
#include "common/timer.h"
#include "model/mine-sweeper/mine_map.h"
#include "mine_sweeper_controller.h"

static MineSweeperView *view;
static MenuItemView *menu_item_cursor;

static Timer *timer;
static uint32_t time;

static uint16_t bomb_max_count;
static uint16_t bomb_count;

static bool is_started;

static void handle_cell_mouse_event(const MineCell *cell, EventMouseType type)
{
    if (type == EVENT_MOUSE_LEFT_CLICK) {
        MineMap_open_cell(cell);
    } else if (type == EVENT_MOUSE_RIGHT_CLICK) {
        if (cell->state == MINE_CELL_UNKNOWN) {
            MineMap_flag_cell(cell);
            bomb_count--;
        } else if (cell->state == MINE_CELL_FLAGGED) {
            MineMap_unflag_cell(cell);
            bomb_count++;
        }
        MineSweeperView_on_bomb_count_update(view, bomb_count);
    }
}

static void handle_menu_item_mouse_event(MenuItemView *menu_item, EventMouseType type)
{
    if (type == EVENT_MOUSE_MOVE) {
        if (menu_item == menu_item_cursor) {
            return;
        }
        if (!menu_item && menu_item_cursor) {
            MenuItemView_on_release(menu_item_cursor);
            menu_item_cursor = NULL;
            return;
        }
        MenuItemView_on_release(menu_item_cursor);
        MenuItemView_on_focus(menu_item);
        menu_item_cursor = menu_item;
    } else if (type == EVENT_MOUSE_LEFT_CLICK) {
        MenuItemView_on_click(menu_item);
    }
}

static void handle_mouse_event(EventMouse event)
{
    MineMapView *map_view = MineSweeperView_get_map_view(view);

    const MineCell *cell = MineMapView_get_positioned_cell(map_view, event.x, event.y);
    if (cell) {
        handle_cell_mouse_event(cell, event.type);
        if (!is_started) {
            Timer_start(timer, TIMER_SECOND, true);
            is_started = true;
        }
        return;
    }
    
    MenuItemView *menu_item = 
        MineSweeperView_get_positioned_menu_item(view, event.x, event.y);
    if (menu_item || menu_item_cursor) {
        handle_menu_item_mouse_event(menu_item, event.type);
        return;
    }
}

static void handle_event(Controller *super, const Event *event)
{
    if (event->type == EVENT_MOUSE) {
        handle_mouse_event(event->mouse);
    }
}

static void on_mine_map_update(const MineCell *cell)
{
    MineMapView *map_view = MineSweeperView_get_map_view(view);

    MineMapView_on_update(map_view, cell);
}

static void on_min_map_result(bool is_success)
{
    MineMapView *map_view = MineSweeperView_get_map_view(view);

    Timer_stop(timer);

    MineMapView_on_finish(map_view, is_success);
}

static void on_time_tick(void *ctx)
{
    MineSweeperView_on_timer_update(view, time++);
}

int MineSweeperController_init(const MineSweeperSettings *settings, 
                               MineSweeperView *_view)
{
    int err = MineMap_init(settings, on_mine_map_update, on_min_map_result);
    if (err) {
        return -1;
    }
    
    time = 0;
    is_started = false;
    timer = Timer_new(on_time_tick, NULL);
    if (!timer) {
        MineSweeperView_on_erase(view);
        MineMap_deinit();
        return -2;
    }
    
    bomb_max_count = settings->num_bombs;
    bomb_count = settings->num_bombs;
    
    view = _view;
    menu_item_cursor = NULL;
    MineSweeperView_on_draw(view);
    MineSweeperView_on_timer_update(view, time);
    MineSweeperView_on_bomb_count_update(view, bomb_count);
    
    return 0;
}

void MineSweeperController_deinit(void)
{
    MineSweeperView_on_erase(view);
    Timer_delete(timer);
    MineMap_deinit();
}

void MineSweeperController_reset(void)
{
    MineMap_reset();
    
    Timer_stop(timer);
    time = 0;
    is_started = false;
    
    bomb_count = bomb_max_count;
    
    MineSweeperView_on_erase(view);
    MineSweeperView_on_draw(view);
    MineSweeperView_on_timer_update(view, time);
    MineSweeperView_on_bomb_count_update(view, bomb_count);
}

Controller mine_sweeper_controller = {
    .handle_event = handle_event,
};