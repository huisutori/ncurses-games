
/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include "model/screen.h"

int CursesDriver_init(void);
void CursesDriver_deinit(void);

short CursesDriver_get_color_id(ScreenColor background, ScreenColor foreground);