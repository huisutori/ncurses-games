/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include "common/message_queue.h"
#include "event.h"

typedef struct {
    Queue super;
    int head, tail;
    Event events[EVENT_QUEUE_SIZE];
} EventQueue;

static MessageQueue *message_queue;

static const void *queue_pop(Queue *queue)
{
    EventQueue *event_queue = (EventQueue *)queue;
    if (event_queue->head == event_queue->tail) {
        return NULL;
    }
    
    Event *event = &event_queue->events[event_queue->head];
    event_queue->head = (event_queue->head + 1) % EVENT_QUEUE_SIZE;
    
    return event;
}

static int queue_push(Queue *queue, const void *data)
{
    EventQueue *event_queue = (EventQueue *)queue;
    if (event_queue->head == ((event_queue->tail + 1) % EVENT_QUEUE_SIZE)) {
        return -1;   
    }
    event_queue->events[event_queue->tail] = *(const Event *)data;
    event_queue->tail = (event_queue->tail + 1) % EVENT_QUEUE_SIZE;
    
    return 0;
}

int Event_init(void)
{
    static EventQueue queue = {
        .super = {
            .pop = queue_pop,
            .push = queue_push,
        },
        .head = 0,
        .tail = 0,
    };
    
    message_queue = MessageQueue_new((Queue *)&queue);
    if (!message_queue) {
        return -1;
    }
    
    return 0;
}

void Event_deinit(void)
{
    if (message_queue) {
        MessageQueue_delete(message_queue);
        message_queue = NULL;
    }
}

const Event *Event_wait(void)
{
    if (!message_queue) {
        return NULL;
    }
    return (const Event *)MessageQueue_wait(message_queue);
}

int Event_push(const Event *event)
{
    if (!message_queue) {
        return -1;
    }
    return MessageQueue_push(message_queue, event);
}