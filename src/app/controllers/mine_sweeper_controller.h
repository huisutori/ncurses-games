/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

#include "model/controller.h"
#include "model/mine-sweeper/views/mine_sweeper_view.h"
#include "model/mine-sweeper/mine_sweeper_settings.h"

int MineSweeperController_init(const MineSweeperSettings *settings, 
                               MineSweeperView *view);
void MineSweeperController_deinit(void);

void MineSweeperController_reset(void);

extern Controller mine_sweeper_controller;