/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>

typedef enum {
    EVENT_KEY,
    EVENT_MOUSE,
    EVENT_SCREEN,
} EventType;

typedef enum {
    EVENT_KEY_UP,
    EVENT_KEY_DOWN,
    EVENT_KEY_LEFT,
    EVENT_KEY_RIGHT,
    EVENT_KEY_ENTER,
    EVENT_KEY_ESC,
    EVENT_KEY_CHARACTER,
} EventKeyType;

typedef struct {
    EventKeyType type;
    char ch;
} EventKey;

typedef enum {
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_LEFT_CLICK,
    EVENT_MOUSE_RIGHT_CLICK,
} EventMouseType;

typedef struct {
    EventMouseType type;
    int x, y;
} EventMouse;

typedef enum {
    EVENT_SCREEN_UPDATE,
    EVENT_SCREEN_RESIZE,
} EventScreenType;

typedef struct {
    EventScreenType type;
    int width, height;
} EventScreen;

typedef struct {
    EventType type;
    union {
        EventKey key;
        EventMouse mouse;
        EventScreen screen;
    };
} Event;

int Event_init(void);
void Event_deinit(void);

int Event_push(const Event *event);
const Event *Event_wait(void);

#define EVENT_QUEUE_SIZE 5