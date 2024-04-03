/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <wchar.h>
#include <ncurses.h>
#include "port/curses/curses_driver.h"
#include "curses_menu_item_view.h"
#include "curses_mine_map_view.h"
#include "curses_mine_sweeper_view.h"

typedef struct {
    MineSweeperView super;
    MineMapView *map;
    size_t num_rows;
    size_t num_cols;
    WINDOW *window;
    size_t width, height;
    size_t map_width;
    size_t num_menu_items;
    MenuItemView *menu_items[];
} CursesMineSweeperView;

#define MENU_ITEM_WIDTH  (15)
#define MENU_ITEM_HEIGHT (3)

#define MINE_MAP_MARGIN_RIGHT (5)

#define TIMER_CODE  (0x23f0)
#define BOMB_CODE   (0x1f4a3)

static void on_draw(MineSweeperView *super)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    wbkgd(view->window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, 
                                                  SCREEN_COLOR_DARK_BLUE));
                                                  
    mvwprintw(view->window, 0, 1, "%lc", (wint_t)TIMER_CODE);
    mvwprintw(view->window, 0, view->map_width / 2, "%lc", (wint_t)BOMB_CODE);
    wrefresh(view->window);

    MineMapView_on_draw(view->map, view->num_rows, view->num_cols);
    
    for (int i = 0; i < view->num_menu_items; i++) {
        MenuItemView_on_draw(view->menu_items[i]);
    }
}

static void on_erase(MineSweeperView *super)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    MineMapView_on_erase(view->map);
    for (int i = 0; i < view->num_menu_items; i++) {
        MenuItemView_on_erase(view->menu_items[i]);
    }
    wclear(view->window);
    
    touchwin(stdscr);
    refresh();
    touchwin(view->window);
    wrefresh(view->window);
}

static void on_timer_update(MineSweeperView *super, uint32_t time)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    mvwprintw(view->window, 0, 4, "%lu", time);
    wrefresh(view->window);
}

static void on_bomb_count_update(MineSweeperView *super, uint16_t bomb_count)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    mvwprintw(view->window, 0, view->map_width / 2 + 3, "%u  ", bomb_count);
    wrefresh(view->window);
}

static MenuItemView *get_first_menu_item(MineSweeperView *super)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    return view->menu_items[0];
}

static MenuItemView *get_positioned_menu_item(MineSweeperView *super, 
                                              size_t x, size_t y)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    for (int i = 0; i < view->num_menu_items; i++) {
        if (MenuItemView_has_position(view->menu_items[i], x, y)) {
            return view->menu_items[i];
        }
    }
    return NULL;
}

static MineMapView *get_map_view(MineSweeperView *super)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    return view->map;
}

MineSweeperView *CursesMineSweeperView_new(size_t num_rows, size_t num_cols,
                                           size_t num_menu_items, 
                                           MenuItem menu_items[])
{
    CursesMineSweeperView *view = malloc(sizeof(CursesMineSweeperView) + 
                                         sizeof(MenuItemView *) * num_menu_items);
    if (!view) {
        return NULL;
    }
    *view = (CursesMineSweeperView) {
        .super = {
            .on_draw = on_draw,
            .on_erase = on_erase,
            .on_timer_update = on_timer_update,
            .on_bomb_count_update = on_bomb_count_update,
            .get_first_menu_item = get_first_menu_item,
            .get_positioned_menu_item = get_positioned_menu_item,
            .get_map_view = get_map_view,
        },
        .map = &curses_mine_map_view,
        .num_rows = num_rows,
        .num_cols = num_cols,
        .num_menu_items = num_menu_items,
    };
    
    size_t screen_width, screen_height;
    getmaxyx(stdscr, screen_height, screen_width);
    
    size_t map_width = CURSES_MINE_MAP_VIEW_GET_WIDTH(num_cols);
    size_t map_height = CURSES_MINE_MAP_VIEW_GET_HEIGHT(num_rows);

    view->width = map_width + MINE_MAP_MARGIN_RIGHT + MENU_ITEM_WIDTH;
    view->height = map_height + 1;
    view->map_width = map_width;
    
    size_t x = (screen_width - view->width) / 2;
    size_t y = (screen_height - view->height) / 2;
    
    view->window = newwin(view->height, view->width, y, x);
    if (!view->window) {
        goto clean_view;
    }
    
    WINDOW *map_window = derwin(view->window, map_height, 
                                map_width, 1, 0);
    if (!map_window) {
        goto clean_view_window;
    }
    CursesMineMapView_set_window(map_window);
    
    size_t menu_item_total_height = (MENU_ITEM_HEIGHT + 1) * num_menu_items - 1;
    
    size_t menu_item_start_x = map_width + MINE_MAP_MARGIN_RIGHT;
    size_t menu_item_start_y = (view->height - menu_item_total_height) / 2;
    
    for (int i = 0; i < num_menu_items; i++, menu_item_start_y += (MENU_ITEM_HEIGHT + 1)) {
        WINDOW *window = derwin(view->window, MENU_ITEM_HEIGHT, MENU_ITEM_WIDTH, 
                                menu_item_start_y, menu_item_start_x);
        if (!window) {
            goto clean_menu_items;
        }

        view->menu_items[i] = CursesMenuItemView_new(&menu_items[i]);
        if (!view->menu_items[i]) {
            delwin(window);
            goto clean_menu_items;
        }
        CursesMenuItemView_set_window(view->menu_items[i], window);
    }
    
    return (MineSweeperView *)view;

clean_menu_items:
    for (int i = 0; i < num_menu_items; i++) {
        if (!view->menu_items[i]) {
            delwin(CursesMenuItemView_get_window(view->menu_items[i]));
            CursesMenuItemView_delete(view->menu_items[i]);
        }
    }
clean_view_window:
    delwin(view->window);
clean_view:
    free(view);
    
    return NULL;
}

void CursesMineSweeperView_delete(MineSweeperView *super)
{
    CursesMineSweeperView *view = (CursesMineSweeperView *)super;
    
    for (int i = 0; i < view->num_menu_items; i++) {
        delwin(CursesMenuItemView_get_window(view->menu_items[i]));
        CursesMenuItemView_delete(view->menu_items[i]);
    }
    delwin(CursesMineMapView_get_window());
    delwin(view->window);
    free(view);
}