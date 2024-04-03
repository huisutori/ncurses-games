/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include <stddef.h>
#include "model/controller.h"
#include "model/menu/views/menu_view.h"

Controller *MenuController_new(MenuView *menu_view);
void MenuController_delete(Controller *controller);