/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <string.h>
#include "port/curses/curses_driver.h"
#include "curses_menu_item_view.h"

typedef struct {
    MenuItemView super;
    WINDOW *window;
    const char *content;
    MenuItemHandler on_click;
    void *ctx;
} CursesMenuItemView;

static void on_draw(MenuItemView *super)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;

    wbkgd(view->window, CursesDriver_get_color_id(CURSES_MENU_ITEM_VIEW_COLOR,
                                                  CURSES_MENU_ITEM_VIEW_CONTENT_COLOR));
    
    size_t width, height;
    getmaxyx(view->window, height, width);
    
    size_t x = (width - strlen(view->content)) / 2;
    size_t y = height / 2;
    mvwprintw(view->window, y, x, "%s", view->content);
    
    wrefresh(view->window);
}

static void on_erase(MenuItemView *super)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;
    
    wclear(view->window);
    
    wrefresh(view->window);
}

static void on_focus(MenuItemView *super)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;

    wbkgd(view->window, CursesDriver_get_color_id(CURSES_MENU_ITEM_VIEW_FOCUSED_COLOR, 
                                                  CURSES_MENU_ITEM_VIEW_CONTENT_COLOR));
    wrefresh(view->window);
}

static void on_release(MenuItemView *super)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;
    
    wbkgd(view->window, CursesDriver_get_color_id(CURSES_MENU_ITEM_VIEW_COLOR, 
                                                  CURSES_MENU_ITEM_VIEW_CONTENT_COLOR));
    wrefresh(view->window);
}

static void on_click(MenuItemView *super)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;
    
    if (view->on_click) {
        view->on_click(view->ctx);
    }
}

static bool has_position(MenuItemView *super, size_t x, size_t y)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;

    int start_x, start_y;
    getbegyx(view->window, start_y, start_x);
    
    int width, height;
    getmaxyx(view->window, height, width);
    
    if (x > start_x && y > start_y && 
        x < start_x + width && y < start_y + height) {
        return true;
    }
    return false;
}

MenuItemView *CursesMenuItemView_new(const MenuItem *item)
{
    CursesMenuItemView *view = malloc(sizeof(CursesMenuItemView));
    if (!view) {
        return NULL;
    }
    *view = (CursesMenuItemView) {
        .super = {
            .on_draw = on_draw,
            .on_erase = on_erase,
            .on_focus = on_focus,
            .on_release = on_release,
            .on_click = on_click,
            .has_position = has_position,
        },
        .content = item->content,
        .on_click = item->on_click,
        .ctx = item->ctx,
    };
    
    return (MenuItemView *)view;
}

void CursesMenuItemView_delete(MenuItemView *view)
{
    free(view);
}

void CursesMenuItemView_set_window(MenuItemView *super, WINDOW *window)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;
    
    view->window = window;
}

WINDOW *CursesMenuItemView_get_window(MenuItemView *super)
{
    CursesMenuItemView *view = (CursesMenuItemView *)super;
    
    return view->window;
}