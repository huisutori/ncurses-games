/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <signal.h>
#include <unistd.h>
#include "model/menu/menu.h"
#include "app/controllers/menu_controller.h"
#include "port/curses/views/curses_menu_view.h"
#include "mine_sweeper_menu_screen.h"
#include "main_menu_screen.h"

typedef enum {
    MAIN_MENU_ITEM_MINE_SWEEPER,
    MAIN_MENU_ITEM_EXIT,
    MAIN_MENU_ITEM_MAX,
} MainMenuItem;

#define MAIN_MENU_ITEM_WIDTH    (25)
#define MAIN_MENU_ITEM_HEIGHT   (3)

static void play_mine_sweeper(void *arg)
{
    Screen_set_current(&mine_sweeper_menu_screen, NULL);
}

static void exit_game(void *arg)
{
    kill(getpid(), SIGTERM);
}

static Menu menu = {
    .title = "Welcome to C Games",
    .num_cols = 1,
    .num_rows = 2,
    .items = {
        [MAIN_MENU_ITEM_MINE_SWEEPER] = {
            .content = "Mine Sweeper",
            .on_click = play_mine_sweeper
        },
        [MAIN_MENU_ITEM_EXIT] = {
            .content = "Exit",
            .on_click = exit_game,
        },
    },
};

static MenuView *menu_view;

static Controller *create_controller(void *ctx)
{
    menu_view = CursesMenuView_new(&menu, MAIN_MENU_ITEM_WIDTH,
                                   MAIN_MENU_ITEM_HEIGHT);
    if (!menu_view) {
        return NULL;
    }
    
    return MenuController_new(menu_view);
}

static void destroy_controller(Controller *controller)
{
    MenuController_delete(controller);
    CursesMenuView_delete(menu_view);
}

Screen main_menu_screen = {
    .create_controller = create_controller,
    .destroy_controller = destroy_controller,
};