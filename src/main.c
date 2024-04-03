/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "model/event.h"
#include "model/screen.h"
#include "app/main_menu_screen.h"
#include "port/curses/curses_driver.h"

static pthread_t exit_thread;
static sigset_t exit_signal;

static void on_exit(void)
{
    Screen_deinit();
    CursesDriver_deinit();
    Event_deinit();
}

static void *exit_thread_func(void *arg)
{
    sigwait(&exit_signal, NULL);

    exit(0);

    return NULL;
}

static int exit_thread_init(void)
{
    sigemptyset(&exit_signal);
    sigaddset(&exit_signal, SIGTERM);
    
    int err = pthread_create(&exit_thread, NULL, 
                             exit_thread_func, NULL);
    if (err) {
        return -1;
    }
    
    pthread_sigmask(SIG_BLOCK, &exit_signal, NULL);
    
    return 0;
}

static void exit_thread_deinit(void)
{
    pthread_cancel(exit_thread);
}

int main(void)
{
    setlocale(LC_ALL, "");
    
    int err = exit_thread_init();
    if (err) {
        return -1;
    }

    err = Event_init();
    if (err) {
        goto clean;
    }
    
    err = CursesDriver_init();
    if (err) {
        goto clean_event;
    }
    
    err = Screen_init(&main_menu_screen, NULL);
    if (err) {
        goto clean_curses;
    }
    
    atexit(on_exit);
    
    pthread_join(exit_thread, NULL);
    
    return 0;

clean_curses:
    CursesDriver_deinit();
clean_event:
    Event_deinit();
clean:
    exit_thread_deinit();
    
    return -1;
}