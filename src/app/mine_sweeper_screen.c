/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "app/controllers/mine_sweeper_controller.h"
#include "port/curses/views/curses_mine_sweeper_view.h"
#include "mine_sweeper_screen.h"

static MineSweeperView *view;

static void on_reset(void *arg)
{
    MineSweeperController_reset();
}

static void on_exit(void *arg)
{
    Screen_set_main(NULL);   
}

#define NUM_MENU_ITEMS 2
static MenuItem menu_items[] = {
    {
        .content = "Reset",
        .on_click = on_reset,
    },
    {
        .content = "Exit",
        .on_click = on_exit,
    },
};


static Controller *create_controller(void *ctx)
{
    MineSweeperSettings *settings = &mine_sweeper_settings[MINE_SWEEPER_LEVEL_EASY];
    if (ctx) {
        settings = ctx;
    }
    
    view = CursesMineSweeperView_new(settings->num_rows, settings->num_cols, 
                                     NUM_MENU_ITEMS, menu_items);
    if (!view) {
        return NULL;
    }
    
    int err = MineSweeperController_init(settings, view);
    if (err) {
        CursesMineSweeperView_delete(view);
        return NULL;
    }
    
    return &mine_sweeper_controller;
}

static void destroy_controller(Controller *controller)
{
    MineSweeperController_deinit();
    CursesMineSweeperView_delete(view);
}

Screen mine_sweeper_screen = {
    .create_controller = create_controller,
    .destroy_controller = destroy_controller,
};