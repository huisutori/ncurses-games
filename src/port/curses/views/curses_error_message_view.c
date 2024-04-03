/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <ncurses.h>
#include "port/curses/curses_driver.h"

void CursesErrorMessageView_show(const char *msg)
{
    attron(CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, 
                                     SCREEN_COLOR_RED));
    mvprintw(0, 0, "%s", msg);
    attroff(CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, 
                                     SCREEN_COLOR_RED));
    
    refresh();
}

void CursesErrorMessageView_hide(void)
{
    move(0, 0);
    clrtoeol();
    
    refresh();
}