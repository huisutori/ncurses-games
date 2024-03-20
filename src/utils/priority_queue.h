/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct PQueue PQueue;

PQueue *PQueue_new(size_t size, bool (*compare)(const void *, const void *));
void PQueue_delete(PQueue *pqueue);

int PQueue_push(PQueue *pqueue, void *data);
void *PQueue_pop(PQueue *pqueue);
void *PQueue_peek(PQueue *pqueue);
void PQueue_erase(PQueue *pqueue, void *data);

bool PQueue_is_empty(PQueue *pqueue);
bool PQueue_is_full(PQueue *pqueue);