/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

typedef struct Thread Thread;
typedef void (*ThreadFunc)(void *arg);

Thread *Thread_new(ThreadFunc func, void *arg);
void Thread_delete(Thread *thread);

int Thread_join(Thread *thread);
int Thread_detach(Thread *thread);