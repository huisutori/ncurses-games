/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "model/menu/menu.h"
#include "app/controllers/menu_controller.h"
#include "model/mine-sweeper/mine_sweeper_settings.h"
#include "port/curses/views/curses_menu_view.h"
#include "mine_sweeper_screen.h"
#include "mine_sweeper_menu_screen.h"

typedef enum {
    MINE_SWEEPER_MENU_ITEM_EASY,
    MINE_SWEEPER_MENU_ITEM_MEDIUM,
    MINE_SWEEPER_MENU_ITEM_HARD,
    MINE_SWEEPER_MENU_ITEM_SCORE,
    MINE_SWEEPER_MENU_ITEM_MAX,
} MineSweeperMenuItem;

#define MINE_SWEEPER_MENU_ITEM_WIDTH    (25)
#define MINE_SWEEPER_MENU_ITEM_HEIGHT   (3)

static void start_game(void *ctx)
{
    Screen_set_current(&mine_sweeper_screen, ctx);
}

static Menu menu = {
    .title = "Mine Sweeper Game",
    .num_rows = 2,
    .num_cols = 2,
    .items = {
        [MINE_SWEEPER_MENU_ITEM_EASY] = {
            .content = "Easy",
            .on_click = start_game,
            .ctx = &mine_sweeper_settings[MINE_SWEEPER_LEVEL_EASY],
        },
        [MINE_SWEEPER_MENU_ITEM_MEDIUM] = {
            .content = "Medium",
            .on_click = start_game,
            .ctx = &mine_sweeper_settings[MINE_SWEEPER_LEVEL_MEDIUM],
        },
        [MINE_SWEEPER_MENU_ITEM_HARD] = {
            .content = "Hard",
            .on_click = start_game,
            .ctx = &mine_sweeper_settings[MINE_SWEEPER_LEVEL_HARD],
        },
        [MINE_SWEEPER_MENU_ITEM_SCORE] = {
            .content = "Score",
        },
    },
};

static MenuView *menu_view;

static Controller *create_controller(void *ctx)
{
    menu_view = CursesMenuView_new(&menu, MINE_SWEEPER_MENU_ITEM_WIDTH,
                                   MINE_SWEEPER_MENU_ITEM_HEIGHT);
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

Screen mine_sweeper_menu_screen = {
    .create_controller = create_controller,
    .destroy_controller = destroy_controller,
};