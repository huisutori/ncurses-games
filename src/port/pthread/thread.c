/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <pthread.h>
#include "common/thread.h"

typedef struct Thread {
    pthread_t super;
} Thread;

Thread *Thread_new(ThreadFunc func, void *arg)
{
    Thread *thread = malloc(sizeof(Thread));
    if (!thread) {
        return NULL;
    }
    
    int err = pthread_create((pthread_t *)thread, NULL, 
                             (void * (*)(void *))func, arg);
    if (err) {
        free(thread);
        return NULL;
    }
    
    return thread;
}

void Thread_delete(Thread *thread)
{
    if (pthread_equal(pthread_self(), *(pthread_t *)thread)) {
        pthread_exit(NULL);
    } else {
        pthread_cancel(*(pthread_t *)thread);
    }
    free(thread);
}

int Thread_join(Thread *thread)
{
    int err = pthread_join(*(pthread_t *)thread, NULL);
    if (err) {
        return -1;
    }
    return 0;
}

int Thread_detach(Thread *thread)
{
    int err = pthread_detach(*(pthread_t *)thread);
    if (err) {
        return -1;
    }
    return 0;
}