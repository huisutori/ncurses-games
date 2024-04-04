/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "port/curses/curses_driver.h"
#include "curses_error_message_view.h"
#include "curses_menu_item_view.h"
#include "curses_menu_view.h"

typedef struct {
    MenuView super;
    WINDOW *window;
    bool is_enabled;
    const char *title;
    size_t width, height;
    size_t item_width, item_height;
    size_t num_items;
    MenuItemView *items[];
} CursesMenuView;

#define MENU_ITEM_MARGIN_X 2
#define MENU_ITEM_MARGIN_Y 1
#define MENU_MARGIN_X 12
#define MENU_MARGIN_Y 6

#define SCREEN_OVERFLOW_MESSAGE "Please increase terminal size"

static bool is_screen_overflow(CursesMenuView *view)
{
    size_t screen_width, screen_height;
    getmaxyx(stdscr, screen_height, screen_width);
    
    if (screen_width > view->width && screen_height > view->height) {
        return false;
    }
    return true;
}

static void on_draw(MenuView *super)
{
    CursesMenuView *view = (CursesMenuView *)super;

    if (is_screen_overflow(view)) {
        CursesErrorMessageView_show(SCREEN_OVERFLOW_MESSAGE);
        view->is_enabled = false;
        return;
    }
    view->is_enabled = true;
    
    wbkgd(view->window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT,
                                                  CURSES_MENU_VIEW_COLOR));
    box(view->window, 0, 0);
    
    size_t title_x = (view->width - strlen(view->title) - 2) / 2;
    mvwprintw(view->window, 0, title_x, " %s ", view->title);
    
    for (int i = 0; i < view->num_items; i++) {
        MenuItemView_on_draw(view->items[i]);
    }
    
    touchwin(stdscr);
    refresh();
    touchwin(view->window);
    wrefresh(view->window);
    for (int i = 0; i < view->num_items; i++) {
        wrefresh(CursesMenuItemView_get_window(view->items[i]));
    }
}

static void on_erase(MenuView *super)
{
    CursesMenuView *view = (CursesMenuView *)super;

    for (int i = 0; i < view->num_items; i++) {
        MenuItemView_on_erase(view->items[i]);
    }

    wclear(view->window);

    touchwin(stdscr);
    refresh();
    touchwin(view->window);
    wrefresh(view->window);
    for (int i = 0; i < view->num_items; i++) {
        wrefresh(CursesMenuItemView_get_window(view->items[i]));
    }
}

static void on_screen_resize(MenuView *super, size_t width, size_t height)
{
    CursesMenuView *view = (CursesMenuView *)super;
    
    bool is_overflow = is_screen_overflow(view);
    if (view->is_enabled && is_overflow) {
        MenuView_on_erase(super);
        CursesErrorMessageView_show(SCREEN_OVERFLOW_MESSAGE);
        
        view->is_enabled = false;
    } else if (!view->is_enabled && !is_overflow) {
        CursesErrorMessageView_hide();

        size_t prev_x, prev_y;
        getbegyx(view->window, prev_y, prev_x);

        size_t x = (width - view->width) / 2;
        size_t y = (height - view->height) / 2;
        
        view->window->_begx = x;
        view->window->_begy = y;
        wresize(view->window, view->height, view->width);
        
        int dx = x - prev_x;
        int dy = y - prev_y;
        
        for (int i = 0; i < view->num_items; i++) {
            WINDOW *window = CursesMenuItemView_get_window(view->items[i]);
            window->_begx += dx;
            window->_begy += dy;
            wresize(window, view->item_height, view->item_width);
        }
        
        MenuView_on_draw(super);
        view->is_enabled = true;
    } else if (view->is_enabled && !is_overflow) {
        size_t prev_x, prev_y;
        getbegyx(view->window, prev_y, prev_x);
    
        size_t x = (width - view->width) / 2;
        size_t y = (height - view->height) / 2;
        mvwin(view->window, y, x);
        
        int dx = x - prev_x;
        int dy = y - prev_y;
        
        for (int i = 0; i < view->num_items; i++) {
            WINDOW *window = CursesMenuItemView_get_window(view->items[i]);
            window->_begx += dx;
            window->_begy += dy;
        }

        touchwin(stdscr);
        refresh();
        wrefresh(view->window);
    }
}

static MenuItemView *get_first_item(MenuView *super)
{
    CursesMenuView *view = (CursesMenuView *)super;
    
    return view->items[0];
}

static MenuItemView *get_positioned_item(MenuView *super, size_t x, size_t y)
{
    CursesMenuView *view = (CursesMenuView *)super;

    for (int i = 0; i < view->num_items; i++) {
        if (MenuItemView_has_position(view->items[i], x, y)) {
            return view->items[i];
        }
    }
    return NULL;
}

static void init_item_neighbors(CursesMenuView *view, size_t num_rows, size_t num_cols)
{
    size_t num_items = num_rows * num_cols;
    
    for (int i = 0; i < num_items; i++) {
        int up_index = (i == 0 ? num_items - 1 : i - num_cols);
        if (up_index < 0) {
            up_index += (num_items - 1);
        }
        int down_index = ((i == num_items - 1) ? 0 : i + num_cols);
        if (down_index >= num_items) {
            down_index -= (num_items - 1);
        } 
        int left_index = i - 1;
        if (left_index < 0) {
            left_index = num_items - 1;
        }
        int right_index = i + 1;
        if (right_index >= num_items) {
            right_index = 0;
        }
        view->items[i]->up = view->items[up_index];
        view->items[i]->down = view->items[down_index];
        view->items[i]->left = view->items[left_index];
        view->items[i]->right = view->items[right_index];
    }
}

MenuView *CursesMenuView_new(const Menu *menu, size_t item_width, size_t item_height)
{
    size_t num_items = menu->num_rows * menu->num_cols;
    
    CursesMenuView *view = malloc(sizeof(CursesMenuView) + 
                                  sizeof(MenuItemView *) * num_items);
    if (!view) {
        return NULL;
    }
    *view = (CursesMenuView) {
        .super = {
            .on_draw = on_draw,
            .on_erase = on_erase,
            .on_screen_resize = on_screen_resize,
            .get_first_item = get_first_item,
            .get_positioned_item = get_positioned_item,
        },
        .title = menu->title,
        .num_items = num_items,
        .item_width = item_width,
        .item_height = item_height,
    };
    
    for (int i = 0; i < num_items; i++) {
        view->items[i] = CursesMenuItemView_new(&menu->items[i]);
        if (!view->items[i]) {
            goto clean_view;
        }
    }
    
    init_item_neighbors(view, menu->num_rows, menu->num_cols);
    
    size_t item_total_width = (item_width + MENU_ITEM_MARGIN_X) * 
                               menu->num_cols - MENU_ITEM_MARGIN_X;
    size_t item_total_height = (item_height + MENU_ITEM_MARGIN_Y) * 
                                menu->num_rows - MENU_ITEM_MARGIN_Y;
    
    view->width = item_total_width + MENU_MARGIN_X;
    view->height = item_total_height + MENU_MARGIN_Y;
    
    size_t screen_width, screen_height;
    getmaxyx(stdscr, screen_height, screen_width);
    
    size_t x, y;
    if (screen_width < view->width || screen_height < view->height) {
        x = y = 0;
    } else {
        x = (screen_width - view->width) / 2;
        y = (screen_height - view->height) / 2;
    }
    
    view->window = newwin(view->height, view->width, y, x);
    if (!view->window) {
        goto clean_items;
    }

    size_t item_x = (view->width - item_total_width) / 2;
    size_t item_y = (view->height - item_total_height) / 2;
    
    for (int i = 0; i < num_items; i++) {
        size_t col = i % menu->num_cols;
        size_t row = i / menu->num_cols;
        
        size_t x = item_x + col * (item_width + MENU_ITEM_MARGIN_X);
        size_t y = item_y + row * (item_height + MENU_ITEM_MARGIN_Y);
        
        WINDOW *item_window = derwin(view->window, item_height, item_width, y, x);
        if (!item_window) {
            goto clean_item_windows;
        }
        CursesMenuItemView_set_window(view->items[i], item_window);
    }

    return (MenuView *)view;

clean_item_windows:
    for (int i = 0; i < num_items; i++) {
        WINDOW *window = CursesMenuItemView_get_window(view->items[i]);
        if (window) {
            delwin(window);
        }
    }

clean_window:
    delwin(view->window);
clean_items:
    for (int i = 0; i < num_items; i++) {
        if (view->items[i]) {
            CursesMenuItemView_delete(view->items[i]);
        }
    }
clean_view:
    free(view);

    return NULL;
}

void CursesMenuView_delete(MenuView *super)
{
    CursesMenuView *view = (CursesMenuView *)super;

    for (int i = 0; i < view->num_items; i++) {
        delwin(CursesMenuItemView_get_window(view->items[i]));
        CursesMenuItemView_delete(view->items[i]);
    }
    delwin(view->window);
    free(view);
}