/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include "event.h"

typedef struct Controller {
    void (*handle_event)(struct Controller *controller, const Event *event);
} Controller;

static inline void Controller_handle_event(Controller *controller, 
                                           const Event *event)
{
    if (controller->handle_event) {
        controller->handle_event(controller, event);
    }
}