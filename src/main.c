/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include "model/event.h"
#include "model/screen.h"
#include "port/curses/curses_driver.h"

int main(void)
{
    Event_init();
    
    CursesDriver_init();
    
    Screen_init(NULL);
    
    Thread_join(screen_thread);
    Thread_join(curses_driver_thread);
    
    Screen_deinit();
    CursesDriver_deinit();
    Event_deinit();
    
    return 0;
}