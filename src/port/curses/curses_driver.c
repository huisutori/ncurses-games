/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>
#include "model/event.h"
#include "common/thread.h"
#include "common/timer.h"
#include "curses_driver.h"

static Thread *curses_driver_thread;

#define COLOR_SCALE(hex) (hex / (float)0xff * 1000) 

static struct {
    short r, g, b;
} colors[SCREEN_COLOR_MAX] = {
    [SCREEN_COLOR_RED]        = { COLOR_SCALE(0xd2), COLOR_SCALE(0x14), COLOR_SCALE(0x04) },
    [SCREEN_COLOR_ORANGE]     = { COLOR_SCALE(0xfc), COLOR_SCALE(0x6a), COLOR_SCALE(0x03) },
    [SCREEN_COLOR_YELLOW]     = { COLOR_SCALE(0xff), COLOR_SCALE(0xbf), COLOR_SCALE(0x00) },
    [SCREEN_COLOR_GREEN]      = { COLOR_SCALE(0x22), COLOR_SCALE(0x8b), COLOR_SCALE(0x22) },
    [SCREEN_COLOR_BLUE]       = { COLOR_SCALE(0x2e), COLOR_SCALE(0x59), COLOR_SCALE(0x84) },
    [SCREEN_COLOR_DARK_BLUE]  = { COLOR_SCALE(0x1c), COLOR_SCALE(0x2e), COLOR_SCALE(0x4a) },
    [SCREEN_COLOR_PURPLE]     = { COLOR_SCALE(0x8f), COLOR_SCALE(0x00), COLOR_SCALE(0xff) },
};


static Timer *screen_event_timer;
static size_t screen_width, screen_height;

static void invoke_screen_event(void *arg)
{
    Event event = {
        .type = EVENT_SCREEN,
        .screen = {
            .type = EVENT_SCREEN_RESIZE,
            .width = screen_width,
            .height = screen_height,
        },
    };

    Event_push(&event);
}

static void on_terminal_resize(int unused)
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    
    resizeterm(size.ws_row, size.ws_col);
    
    int dummy = 0;
    ungetch(dummy);

    screen_width = size.ws_col;
    screen_height = size.ws_row;

    if (!Timer_is_running(screen_event_timer)) {
        Timer_start(screen_event_timer, 500, false);
    }
}

static void curses_driver_thread_func(void *arg)
{
    while (!isendwin()) {
        Event event;
        int ch = getch();
        
        if (ch == KEY_MOUSE) {
            MEVENT mouse_event;
            getmouse(&mouse_event);
            
            event.type = EVENT_MOUSE;
            event.mouse.x = mouse_event.x;
            event.mouse.y = mouse_event.y;

            if (mouse_event.bstate == BUTTON1_CLICKED) {
                event.mouse.type = EVENT_MOUSE_LEFT_CLICK;
            } else if (mouse_event.bstate == BUTTON3_CLICKED) {
                event.mouse.type = EVENT_MOUSE_RIGHT_CLICK;
            } else {
                event.mouse.type = EVENT_MOUSE_MOVE;
                static int mouse_x, mouse_y;
                if (event.mouse.x == mouse_x && event.mouse.y == mouse_y) {
                    continue;
                }
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            }
        } else {
            event.type = EVENT_KEY;
            event.key.ch = (char)ch;
            
            if (ch == KEY_UP) {
                event.key.type = EVENT_KEY_UP;
            } else if (ch == KEY_DOWN) {
                event.key.type = EVENT_KEY_DOWN;
            } else if (ch == KEY_LEFT) {
                event.key.type = EVENT_KEY_LEFT;
            } else if (ch == KEY_RIGHT) {
                event.key.type = EVENT_KEY_RIGHT;
            } else if (ch == KEY_ENTER || ch == '\n') {
                event.key.type = EVENT_KEY_ENTER;
            } else if (ch == KEY_EXIT) {
                event.key.type = EVENT_KEY_ESC;
            } else {
                event.key.type = EVENT_KEY_CHARACTER;
            }
        }
        
        Event_push(&event);
    }
}

int CursesDriver_init(void)
{   
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, true);
    mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED | REPORT_MOUSE_POSITION, NULL);
    printf("\033[?1003h\n");
    signal(SIGWINCH, on_terminal_resize);
    
    start_color();
    for (int i = 0; i < SCREEN_COLOR_MAX; i++) {
        init_color(i + 1, colors[i].r, colors[i].g, colors[i].b);
    }
    for (int i = 0, id = 1; i < SCREEN_COLOR_MAX; i++) {
        for (int j = 0; j < SCREEN_COLOR_MAX; j++, id++) {
            init_pair(id, i + 1, j + 1);
        }
    }
    
    bkgd(CursesDriver_get_color_id(SCREEN_COLOR_DEFAULT, SCREEN_COLOR_DEFAULT));
    refresh();
    
    curses_driver_thread = Thread_new(curses_driver_thread_func, NULL);
    if (!curses_driver_thread) {
        return -1;
    }
    
    screen_event_timer = Timer_new(invoke_screen_event, NULL);
    if (!screen_event_timer) {
        Thread_delete(curses_driver_thread);
        return -2;
    }
    
    return 0;
}

void CursesDriver_deinit(void)
{
    if (screen_event_timer) {
        Timer_delete(screen_event_timer);
    }
    if (curses_driver_thread) {
        Thread_delete(curses_driver_thread);
    }
    endwin();
}

short CursesDriver_get_color_id(ScreenColor background, ScreenColor foreground)
{
    ScreenColor b = (background == SCREEN_COLOR_DEFAULT ? 
                   SCREEN_COLOR_BACKGROUND_DEFAULT : background);
    ScreenColor f = (foreground == SCREEN_COLOR_DEFAULT ?
                   SCREEN_COLOR_FOREGROUND_DEFAULT : foreground);
    
    return COLOR_PAIR(f * SCREEN_COLOR_MAX + b + 1);
}