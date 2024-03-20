/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <pthread.h>
#include "common/mutex.h"

typedef struct Mutex {
    pthread_mutex_t super;
} Mutex;

Mutex *Mutex_new(void)
{
    Mutex *mutex = malloc(sizeof(Mutex));
    if (!mutex) {
        return NULL;
    }
    pthread_mutex_init((pthread_mutex_t *)mutex, NULL);
    
    return mutex;
}

void Mutex_delete(Mutex *mutex)
{
    pthread_mutex_destroy((pthread_mutex_t *)mutex);
    free(mutex);
}

void Mutex_lock(Mutex *mutex)
{
    pthread_mutex_lock((pthread_mutex_t *)mutex);
}

void Mutex_unlock(Mutex *mutex)
{
    pthread_mutex_unlock((pthread_mutex_t *)mutex);
}