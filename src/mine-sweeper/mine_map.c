/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mine_map.h"

typedef struct MineMap {
    uint8_t num_rows;
    uint8_t num_cols;
    MineCell **cells;
} MineMap;

static void implant_bombs(MineMap *map, uint8_t num_bombs)
{
    srand(time(NULL));
 
    uint8_t bombs = 0;
    while (bombs < num_bombs) {
        uint8_t x = rand() % map->num_rows;
        uint8_t y = rand() % map->num_cols;

        if (!map->cells[x][y].is_bomb) {
            map->cells[x][y].is_bomb = true;
            bombs++;
        }
    }
}

static void update_bomb_count(MineMap *map)
{
    static const int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static const int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < map->num_rows; i++) {
        for (int j = 0; j < map->num_cols; j++) {
            for (int k = 0; k < 8; k++) {
                int x = i + dx[k];
                int y = j + dy[k];
                if (x < 0 || y < 0 || x >= map->num_rows || y >= map->num_cols) {
                    continue;
                }
                if (map->cells[x][y].is_bomb) {
                    map->cells[i][j].bomb_count++;
                }
            }
        }
    }
}

static void init_map(MineMap *map, uint8_t num_bombs)
{
    for (int i = 0; i < map->num_rows; i++) {
        for (int j = 0; j < map->num_cols; j++) {
            map->cells[i][j].state = MINE_CELL_UNKNOWN;
            map->cells[i][j].bomb_count = 0;
            map->cells[i][j].is_bomb = false;
        }
    }
    
    implant_bombs(map, num_bombs);
    update_bomb_count(map);
}

MineMap *MineMap_new(uint8_t num_rows, uint8_t num_cols, uint8_t num_bombs)
{
    MineMap *map = calloc(1, sizeof(MineMap));
    if (!map) {
        return NULL;
    }
    map->num_rows = num_rows;
    map->num_cols = num_cols;
    
    map->cells = calloc(1, sizeof(MineCell *) * num_cols);
    if (!map->cells) {
        goto delete_map;
    }
    for (int i = 0; i < num_cols; i++) {
        map->cells[i] = calloc(1, sizeof(MineCell) * num_rows);
        if (!map->cells[i]) {
            goto delete_cells;
        }
    }
    
    init_map(map, num_bombs);
    
    return map;

delete_cells:
    for (int i = 0; i < num_cols; i++) {
        if (map->cells[i]) {
            free(map->cells[i]);
        } else {
            break;
        }
    }
    if (map->cells) {
        free(map->cells);
    }

delete_map:
    free(map);
    
    return NULL;
}

void MineMap_delete(MineMap *map)
{
    for (int i = 0; i < map->num_cols; i++) {
        free(map->cells[i]);
    }
    free(map->cells);
    free(map);
}

static void open_cell(MineMap *map, int x, int y)
{
    if (x < 0 || y < 0 || x >= map->num_rows || y >= map->num_cols) {
        return;
    }
    if (map->cells[x][y].state != MINE_CELL_UNKNOWN) {
        return;
    }
    if (map->cells[x][y].is_bomb) {
        return;
    }
    
    map->cells[x][y].state = MINE_CELL_OPENED;
    if (map->cells[x][y].bomb_count != 0) {
        return;
    }

    static const int dx[4] = { 0, 0, -1, 1};
    static const int dy[4] = { -1, 1, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        open_cell(map, x + dx[i], y + dy[i]);
    }
}

static void open_map(MineMap *map)
{
    for (int i = 0; i < map->num_rows; i++) {
        for (int j = 0; j < map->num_cols; j++) {
            map->cells[i][j].state = MINE_CELL_OPENED;
        }
    }
}

bool MineMap_open_cell(MineMap *map, uint8_t x, uint8_t y)
{
    if (x >= map->num_rows || y >= map->num_cols) {
        return true;
    }
    if (map->cells[x][y].state != MINE_CELL_UNKNOWN) {
        return true;
    }
    if (map->cells[x][y].is_bomb) {
        open_map(map);
        return false;
    }
    open_cell(map, x, y);
    
    return true;

}
void MineMap_flag_cell(MineMap *map, uint8_t x, uint8_t y)
{
    if (x >= map->num_rows || y >= map->num_cols) {
        return;
    }
    if (map->cells[x][y].state != MINE_CELL_UNKNOWN) {
        return;
    }
    map->cells[x][y].state = MINE_CELL_FLAGGED;
}