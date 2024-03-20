/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <pthread.h>
#include "common/message_queue.h"

typedef struct MessageQueue {
    Queue *queue;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} MessageQueue;

MessageQueue *MessageQueue_new(Queue *queue)
{
    MessageQueue *message_queue = malloc(sizeof(MessageQueue));
    if (!message_queue) {
        return NULL;
    }
    message_queue->queue = queue;
    pthread_cond_init(&message_queue->cond, NULL);
    pthread_mutex_init(&message_queue->mutex, NULL);
    
    return message_queue;
}

void MessageQueue_delete(MessageQueue *message_queue)
{
    pthread_cond_destroy(&message_queue->cond);
    pthread_mutex_destroy(&message_queue->mutex);
    
    free(message_queue);
}

const void *MessageQueue_wait(MessageQueue *message_queue)
{
    pthread_mutex_lock(&message_queue->mutex);
    
    const void *message = Queue_pop(message_queue->queue);
    if (!message) {
        pthread_cond_wait(&message_queue->cond, &message_queue->mutex);
        message = Queue_pop(message_queue->queue);
    }
    pthread_mutex_unlock(&message_queue->mutex);
    
    return message;
}

const void *MessageQueue_wait_timeout(MessageQueue *message_queue, uint32_t timeout_ms)
{
    pthread_mutex_lock(&message_queue->mutex);
    
    const void *message = Queue_pop(message_queue->queue);
    if (!message) {
        struct timespec timeout = {
            .tv_sec = timeout_ms / 1000,
            .tv_nsec = (timeout_ms % 1000) * 1000000,
        };
        pthread_cond_timedwait(&message_queue->cond, &message_queue->mutex, &timeout);
        message = Queue_pop(message_queue->queue);
    }
    pthread_mutex_unlock(&message_queue->mutex);
    
    return message;
}

int MessageQueue_push(MessageQueue *message_queue, const void *message)
{
    pthread_mutex_lock(&message_queue->mutex);
    
    int err = Queue_push(message_queue->queue, message);
    if (err) {
        pthread_mutex_unlock(&message_queue->mutex);
        return -1;
    };
    
    pthread_cond_signal(&message_queue->cond);
    pthread_mutex_unlock(&message_queue->mutex);
    
    return 0;
}