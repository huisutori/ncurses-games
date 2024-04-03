/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include "common/thread.h"
#include "screen.h"

static Thread *screen_thread;

static const Screen *main_screen;
static const Screen *current_screen;
static void *current_screen_ctx;
static Controller *controller;

static enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_MAX,
} state;

static const Screen *idle_step(const Screen *screen)
{
    if (screen->create_controller) {
        controller = screen->create_controller(current_screen_ctx);
    }
    state = STATE_RUNNING;
    
    return screen;
}

static const Screen *running_step(const Screen *screen)
{
    const Event *event = Event_wait();
    if (!event) {
        return screen;
    }
    
    if (event->type == EVENT_SCREEN && 
        event->screen.type == EVENT_SCREEN_UPDATE) {
        if (screen->destroy_controller) {
            screen->destroy_controller(controller);
        }
        state = STATE_IDLE;
        return current_screen;
    }

    Controller_handle_event(controller, event);
    
    return screen;
}

static const Screen *(*step[STATE_MAX])(const Screen *) = {
    [STATE_IDLE] = idle_step,
    [STATE_RUNNING] = running_step,
};

static void screen_thread_func(void *arg)
{
    const Screen *screen = current_screen;
    state = STATE_IDLE;
    
    while (screen) {
        screen = step[state](screen);
    }
}

int Screen_init(const Screen *_main_screen, void *ctx)
{
    main_screen = _main_screen;
    current_screen = _main_screen;
    current_screen_ctx = ctx;
    
    screen_thread = Thread_new(screen_thread_func, NULL);
    if (!screen_thread) {
        return -1;
    }
    
    return 0;
}

void Screen_deinit(void)
{
    if (screen_thread) {
        Thread_delete(screen_thread);
        if (state == STATE_RUNNING) {
            if (current_screen->destroy_controller) {
                current_screen->destroy_controller(controller);
            }
        }
        main_screen = current_screen = NULL;
        screen_thread = NULL;
    }
}

void Screen_set_main(void *ctx)
{
    current_screen = main_screen;
    current_screen_ctx = ctx;
    
    Event event = {
        .type = EVENT_SCREEN,
        .screen = {
            .type = EVENT_SCREEN_UPDATE,
        },
    };
    Event_push(&event);
}

void Screen_set_current(const Screen *screen, void *ctx)
{
    current_screen = screen;
    current_screen_ctx = ctx;
    
    Event event = {
        .type = EVENT_SCREEN,
        .screen = {
            .type = EVENT_SCREEN_UPDATE,
        },
    };
    Event_push(&event);
}

void Screen_get_size(size_t *width, size_t *height)
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    
    *width = size.ws_col;
    *height = size.ws_row;
}