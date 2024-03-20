/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

typedef struct Semaphore Semaphore;

Semaphore *Semaphore_new(void);
void Semaphore_delete(Semaphore *sem);

void Semaphore_post(Semaphore *sem);
void Semaphore_wait(Semaphore *sem);