/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "semaphore.h"

#define SEMAPHORE_NAME_MAX 10

typedef struct Semaphore {
    sem_t *handle;
    char name[SEMAPHORE_NAME_MAX];
} Semaphore;

static int id;

Semaphore *Semaphore_new(void)
{
    Semaphore *sem = malloc(sizeof(Semaphore));
    if (!sem) {
        return NULL;
    }
    
    sprintf(sem->name, "sem_%d", id++);
    sem_unlink(sem->name);
    
    sem->handle = sem_open(sem->name, O_CREAT, 0600, 1);
    if (sem->handle == SEM_FAILED) {
        free(sem);
        return NULL;
    }
    
    return sem;
}

void Semaphore_delete(Semaphore *sem)
{
    sem_close(sem->handle);
    sem_unlink(sem->name);
    
    free(sem);
}

void Semaphore_post(Semaphore *sem)
{
    sem_post(sem->handle);
}

void Semaphore_wait(Semaphore *sem)
{
    sem_wait(sem->handle);
}