/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

typedef struct Mutex Mutex;

Mutex *Mutex_new(void);
void Mutex_delete(Mutex *mutex);

void Mutex_lock(Mutex *mutex);
void Mutex_unlock(Mutex *mutex);