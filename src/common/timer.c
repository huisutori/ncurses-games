/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include "utils/priority_queue.h"
#include "thread.h"
#include "semaphore.h"
#include "mutex.h"
#include "timer.h"

#define TIMER_QUEUE_SIZE 10

typedef struct Timer {
    TimerCallback callback;
    void *ctx;
    uint32_t timeout_ms;
    uint32_t expire_timeout_ms;
    bool is_periodic;
    bool is_running;
} Timer;

static Thread *timer_thread;
static Semaphore *timer_sem;
static Mutex *timer_mutex;
static PQueue *timer_queue;

static size_t num_timers = 0;
static bool is_running = false;

static void on_alarm(int unused)
{
    if (is_running) {
        Semaphore_post(timer_sem);
    }
}

static void schedule_alarm(uint32_t timeout_ms)
{
    struct itimerval interval_time = (struct itimerval) {
        .it_value = {
            .tv_sec = timeout_ms / 1000,
            .tv_usec = (timeout_ms % 1000) * 1e3,
        },
    };
    setitimer(ITIMER_REAL, &interval_time, NULL);
}

static uint32_t get_current_time_ms(void)
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    
    uint32_t time_ms;
    time_ms = time.tv_sec * 1000;
    time_ms += time.tv_nsec / 1e6;
    
    return time_ms;
}

static bool is_timer_expired(Timer *timer)
{
    return (get_current_time_ms() >= timer->expire_timeout_ms);
}

static void timer_thread_func(void *arg)
{
    signal(SIGALRM, on_alarm);

    while (1) {
        Semaphore_wait(timer_sem);

        Timer *timer = PQueue_peek(timer_queue);
        while (timer && is_timer_expired(timer)) {
            timer = PQueue_pop(timer_queue);
            
            if (timer->callback) {
                timer->callback(timer->ctx);
            }
            
            if (timer->is_periodic) {
                timer->expire_timeout_ms = get_current_time_ms() +
                                           timer->timeout_ms;
                PQueue_push(timer_queue, timer);
            } else {
                timer->is_running = false;
            }
            
            timer = PQueue_peek(timer_queue);
        }
        
        timer = PQueue_peek(timer_queue);
        if (timer) {
            schedule_alarm(timer->expire_timeout_ms - get_current_time_ms());
        }
    }
}

static bool compare_timer(const void *data1, const void *data2)
{
    Timer *timer1 = (Timer *)data1;
    Timer *timer2 = (Timer *)data2;
    
    return timer1->expire_timeout_ms < timer2->expire_timeout_ms;
}

static int start_running(void)
{
    timer_queue = PQueue_new(TIMER_QUEUE_SIZE, compare_timer);
    if (!timer_queue) {
        return -1;
    }
    
    timer_sem = Semaphore_new();
    if (!timer_sem) {
        goto error_sem;
    }
    
    timer_thread = Thread_new(timer_thread_func, NULL);
    if (!timer_thread) {
        goto error_thread;
    }
    
    Thread_detach(timer_thread);
    
    return 0;
    
error_thread:
    Semaphore_delete(timer_sem);
error_sem:
    PQueue_delete(timer_queue);
    
    return -1;
}

static void stop_running(void)
{
    Thread_delete(timer_thread);
    Semaphore_delete(timer_sem);
    PQueue_delete(timer_queue);
}

Timer *Timer_new(TimerCallback callback, void *ctx)
{
    Timer *timer = malloc(sizeof(Timer));
    if (!timer) {
        return NULL;
    }
    *timer = (Timer) {
        .callback = callback,
        .ctx = ctx,
        .is_running = false,
    };
    
    if (num_timers++ == 0) {
        timer_mutex = Mutex_new();
    }
    
    return timer;
}

void Timer_delete(Timer *timer)
{
    if (timer->is_running) {
        Timer_stop(timer);
    }
    free(timer);
    
    if (--num_timers == 0) {
        Mutex_delete(timer_mutex);
    }
}

int Timer_start(Timer *timer, uint32_t timeout_ms, bool is_periodic)
{
    Mutex_lock(timer_mutex);

    if (!is_running) {
        int err = start_running();
        if (err) {
            return -1;
        }
        is_running = true;
    }

    if (timer->is_running) {
        PQueue_erase(timer_queue, timer);
        timer->is_running = false;
    }
    
    timer->timeout_ms = timeout_ms;
    timer->expire_timeout_ms = get_current_time_ms() + timeout_ms;
    timer->is_periodic = is_periodic;

    PQueue_push(timer_queue, timer);
    timer->is_running = true;
    
    if (PQueue_is_empty(timer_queue) ||
        PQueue_peek(timer_queue) == timer) {
        schedule_alarm(timeout_ms);
    }

    Mutex_unlock(timer_mutex);

    return 0;
}

void Timer_stop(Timer *timer)
{
    Mutex_lock(timer_mutex);

    if (timer->is_running) {
        PQueue_erase(timer_queue, timer);
        timer->is_running = false;
    }
    
    if (is_running && PQueue_is_empty(timer_queue)) {
        stop_running();
        is_running = false;
    }
    
    Mutex_unlock(timer_mutex);
}

bool Timer_is_running(Timer *timer)
{
    return timer->is_running;
}