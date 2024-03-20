/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stdlib.h>

typedef struct Queue {
    const void *(*pop)(const struct Queue *queue);
    int (*push)(const struct Queue *queue, const void *data);
} Queue;

static inline const void *Queue_pop(const Queue *queue)
{
    if (queue->pop) {
        return queue->pop(queue);
    }
    return NULL;
}

static inline int Queue_push(const Queue *queue, const void *data)
{
    if (queue->push) {
        return queue->push(queue, data);
    }
    return -1;
}