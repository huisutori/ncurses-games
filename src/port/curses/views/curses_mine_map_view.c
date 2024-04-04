/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <wchar.h>
#include "model/mine-sweeper/mine_map.h"
#include "port/curses/curses_driver.h"
#include "curses_mine_map_view.h"

static WINDOW *window;

#define UNKNOWN_CELL_CODE    (0x25a0)
#define EMPTY_CELL_CODE      (0x25a1)
#define BOMB_CELL_CODE       (0x1f4a3)
#define FLAG_CELL_CODE       (0x1F6A9)
#define WRONG_FLAG_CELL_CODE (0x2715)

#define GET_SCREEN_POS_X(x) ((x) * 2 + 2)
#define GET_SCREEN_POS_Y(y) ((y) + 1)

static ScreenColor bomb_count_get_color(size_t bomb_count)
{
    switch (bomb_count)
    {
    case 2:
        return SCREEN_COLOR_BLUE;
    case 3:
        return SCREEN_COLOR_GREEN;
    case 4:
        return SCREEN_COLOR_ORANGE;
    case 5:
    case 6:
    case 7:
    case 8:
        return SCREEN_COLOR_RED;
    default:
        return SCREEN_COLOR_DARK_BLUE;
    }
}

static void on_draw(MineMapView *view, size_t num_rows, size_t num_cols)
{
    if (!window) {
        return;
    }

    wbkgd(window, CursesDriver_get_color_id(SCREEN_COLOR_YELLOW,
                                            SCREEN_COLOR_DARK_BLUE));
    box(window, 0, 0);
    
    for (int i = 0; i < num_rows; i++) {
        wmove(window, i + 1, 1);
        for (int j = 0; j < num_cols; j++) {
            wprintw(window, " %lc", (wint_t)UNKNOWN_CELL_CODE);
        }
        wprintw(window, " ");
    }
    
    wrefresh(window);
}

static void on_erase(MineMapView *view)
{
    if (!window) {
        return;
    }

    wclear(window);
    wrefresh(window);
}

static void on_update(MineMapView *view, const MineCell *cell)
{
    if (!window) {
        return;
    }

    size_t x = GET_SCREEN_POS_X(cell->x);
    size_t y = GET_SCREEN_POS_Y(cell->y);

    if (cell->state == MINE_CELL_UNKNOWN) {
        mvwprintw(window, y, x, "%lc ", (wint_t)UNKNOWN_CELL_CODE);
    } else if (cell->state == MINE_CELL_FLAGGED) {
        mvwprintw(window, y, x, "%lc", (wint_t)FLAG_CELL_CODE);
    } else if (cell->state == MINE_CELL_ERROR) {
        wattron(window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, 
                                                  SCREEN_COLOR_RED));
        mvwprintw(window, y, x, "%lc ", (wint_t)WRONG_FLAG_CELL_CODE);
        wattroff(window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, 
                                                   SCREEN_COLOR_RED));
    } else {
        if (cell->is_bomb) {
            mvwprintw(window, y, x, "%lc", (wint_t)BOMB_CELL_CODE);
        } else if (cell->bomb_count == 0) {
            mvwprintw(window, y, x, "%lc ", (wint_t)EMPTY_CELL_CODE);
        } else {
            ScreenColor color = bomb_count_get_color(cell->bomb_count);
            wattron(window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, color));
            mvwprintw(window, y, x, "%d", cell->bomb_count);
            wattroff(window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, color));
        }
    }
    
    wrefresh(window);
}

static void on_finish(MineMapView *view, bool is_success)
{
    if (!window) {
        return;
    }

    ScreenColor color;
    if (is_success) {
        color = SCREEN_COLOR_GREEN;
    } else {
        color = SCREEN_COLOR_RED;
    }
    
    wattron(window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, color));
    box(window, 0, 0);
    wattroff(window, CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, color));
    
    wrefresh(window);
}

static const MineCell *get_positioned_cell(MineMapView *view, 
                                           size_t x, size_t y)
{
    if (!window) {
        return NULL;
    }

    size_t window_x, window_y;
    getbegyx(window, window_y, window_x);
    
    size_t window_width, window_height;
    getmaxyx(window, window_height, window_width);

    if (x < window_x || y < window_y || 
        x > window_x + window_width || y > window_y + window_height) {
        return NULL;
    }

    if ((x - window_x) % 2 != 0) {
        return NULL;
    }
    
    size_t cell_x, cell_y;
    cell_x = (x - window_x - 1) / 2;
    cell_y = y - (window_y + 1);
    
    return MineMap_get_cell(cell_x, cell_y);
}

void CursesMineMapView_set_window(WINDOW *_window)
{
    window = _window;
}

WINDOW *CursesMineMapView_get_window(void)
{
    return window;
}

MineMapView curses_mine_map_view = {
    .on_draw = on_draw,
    .on_erase = on_erase,
    .on_update = on_update,
    .on_finish = on_finish,
    .get_positioned_cell = get_positioned_cell,
};