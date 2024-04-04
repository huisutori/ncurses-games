/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>

typedef void (*MenuItemHandler)(void *ctx);

typedef struct {
    const char *content;
    MenuItemHandler on_click;
    void *ctx;
} MenuItem;

typedef struct {
    const char *title;
    size_t num_rows;
    size_t num_cols;
    MenuItem items[];
} Menu;