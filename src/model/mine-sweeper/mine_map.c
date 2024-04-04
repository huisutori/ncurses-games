/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "mine_map.h"

static const MineSweeperSettings *settings;
static bool is_finished;

static MineMapUpdateHandler on_update;
static MineMapResultHandler on_result;

static MineCell **cells;
static size_t num_cells;
static size_t num_opened_cells;

static const int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
static const int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

static void implant_bombs(void)
{
    srand(time(NULL));
 
    size_t bombs = 0;
    while (bombs < settings->num_bombs) {
        size_t x = rand() % settings->num_cols;
        size_t y = rand() % settings->num_rows;

        if (!cells[x][y].is_bomb) {
            cells[x][y].is_bomb = true;
            bombs++;
        }
    }
}

static void update_bomb_count(void)
{
    for (int i = 0; i < settings->num_cols; i++) {
        for (int j = 0; j < settings->num_rows; j++) {
            for (int k = 0; k < 8; k++) {
                int x = i + dx[k];
                int y = j + dy[k];
                if (x < 0 || y < 0 || x >= settings->num_cols || y >= settings->num_rows) {
                    continue;
                }
                if (cells[x][y].is_bomb) {
                    cells[i][j].bomb_count++;
                }
            }
        }
    }
}

static void init_map(void)
{
    num_opened_cells = 0;
    is_finished = false;
    for (int i = 0; i < settings->num_cols; i++) {
        for (int j = 0; j < settings->num_rows; j++) {
            cells[i][j].state = MINE_CELL_UNKNOWN;
            cells[i][j].x = i;
            cells[i][j].y = j;
            cells[i][j].bomb_count = 0;
            cells[i][j].is_bomb = false;
        }
    }
    implant_bombs();
    update_bomb_count();
}

int MineMap_init(const MineSweeperSettings *_settings,  
                 MineMapUpdateHandler _on_update, 
                 MineMapResultHandler _on_result)
{
    assert(_settings);
    assert(_on_update);
    assert(_on_result);

    settings = _settings;
    
    on_update = _on_update;
    on_result = _on_result;
    
    num_cells = settings->num_rows * settings->num_cols;

    cells = calloc(1, sizeof(MineCell *) * settings->num_cols);
    if (!cells) {
        return -1;
    }
    
    for (int i = 0; i < settings->num_cols; i++) {
        cells[i] = calloc(1, sizeof(MineCell) * settings->num_rows);
        if (!cells[i]) {
            goto error;
        }
    }
    init_map();

    return 0;

error:
    for (int i = 0; i < settings->num_cols; i++) {
        if (cells[i]) {
            free(cells[i]);
        }
    }
    free(cells);

    return -1;
}

void MineMap_deinit(void)
{
    for (int i = 0; i < settings->num_cols; i++) {
        free(cells[i]);
    }
    free(cells);
}

void MineMap_reset(void)
{
    init_map();
}

const MineCell *MineMap_get_cell(size_t x, size_t y)
{
    if (x >= settings->num_cols || y >= settings->num_rows) {
        return NULL;
    }
    return &cells[x][y];
}

static void open_cell(int x, int y)
{
    if (x < 0 || y < 0 || x >= settings->num_cols || y >= settings->num_rows) {
        return;
    }
    if (cells[x][y].state != MINE_CELL_UNKNOWN) {
        return;
    }
    if (cells[x][y].is_bomb) {
        return;
    }
    
    cells[x][y].state = MINE_CELL_OPENED;
    on_update(&cells[x][y]);
    
    num_opened_cells++;
    
    if (cells[x][y].bomb_count != 0) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        open_cell(x + dx[i], y + dy[i]);
    }
}

static void open_map(void)
{
    for (int i = 0; i < settings->num_cols; i++) {
        for (int j = 0; j < settings->num_rows; j++) {
            if (cells[i][j].is_bomb) {
                cells[i][j].state = MINE_CELL_OPENED;
                on_update(&cells[i][j]);
                num_opened_cells++;
            } else {
                if (cells[i][j].state == MINE_CELL_FLAGGED) {
                    cells[i][j].state = MINE_CELL_ERROR;
                    on_update(&cells[i][j]);
                }
            }
        }
    }
}

void MineMap_open_cell(const MineCell *cell)
{
    if (is_finished) {
        return;
    }

    size_t x = cell->x;
    size_t y = cell->y;

    if (cell->is_bomb) {
        open_map();

        is_finished = true;
        on_result(false);
    } else {
        open_cell((int)x, (int)y);
        
        if ((num_cells - settings->num_bombs) == num_opened_cells) {
            is_finished = true;
            on_result(true);
        }
    }
}

void MineMap_flag_cell(const MineCell *cell)
{
    if (cell->state != MINE_CELL_UNKNOWN) {
        return;
    }
    size_t x = cell->x;
    size_t y = cell->y;
    
    cells[x][y].state = MINE_CELL_FLAGGED;
    on_update(cell);
}

void MineMap_unflag_cell(const MineCell *cell)
{
    if (cell->state != MINE_CELL_FLAGGED) {
        return;
    }
    size_t x = cell->x;
    size_t y = cell->y;
    
    cells[x][y].state = MINE_CELL_UNKNOWN;
    on_update(cell);
}

void MineMap_get_size(size_t *num_rows, size_t *num_cols)
{
    *num_rows = settings->num_rows;
    *num_cols = settings->num_cols;
}