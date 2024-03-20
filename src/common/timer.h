/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct Timer Timer;
typedef void (*TimerCallback)(void *);

Timer *Timer_new(TimerCallback callback, void *ctx);
void Timer_delete(Timer *timer);

int Timer_start(Timer *timer, uint32_t timeout_ms, bool is_periodic);
void Timer_stop(Timer *timer);