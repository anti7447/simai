#pragma once

#define CELL_SIZE 40
#define MAP_HEIGHT 10
#define MAP_WIDTH 10

typedef enum {
    Tile_Void = 2,
    Tile_Free = 0,
    Tile_Stone = 1,
    Tile_Berry = 3,
} TileType;

typedef struct {
    TileType type;
    void *owner; // Yet only Person point
} Cell;

typedef struct {
    Cell Grid[MAP_HEIGHT][MAP_WIDTH];
} Map;

Map MapInit();

int MapDraw(Map*);

int MapCellIsFree(Map*, int x, int y);
