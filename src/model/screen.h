/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include "common/thread.h"
#include "controller.h"

typedef struct {
    Controller *(*create_controller)(void);
    void (*destroy_controller)(Controller *);
} Screen;

typedef enum {
    SCREEN_COLOR_RED,
    SCREEN_COLOR_ORANGE,
    SCREEN_COLOR_YELLOW,
    SCREEN_COLOR_GREEN,
    SCREEN_COLOR_BLUE,
    SCREEN_COLOR_DARK_BLUE,
    SCREEN_COLOR_PURPLE,
    SCREEN_COLOR_MAX,
    SCREEN_COLOR_DEFAULT,
} ScreenColor;

#define SCREEN_COLOR_BACKGROUND_DEFAULT SCREEN_COLOR_YELLOW
#define SCREEN_COLOR_FOREGROUND_DEFAULT SCREEN_COLOR_RED

int Screen_init(const Screen *main_screen);
void Screen_deinit(void);

void Screen_set_main(void);
void Screen_set_current(const Screen *screen);

void Screen_get_size(size_t *width, size_t *height);

extern Thread *screen_thread;