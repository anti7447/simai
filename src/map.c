#include <stddef.h>
#include <raylib.h>
#include "map.h"

Map MapInit() {
    Map map;
    for (int j = 0; j < MAP_HEIGHT; j++) {
        for (int i = 0; i < MAP_WIDTH; i++) {
            map.Grid[j][i] = (Cell) { Tile_Free, NULL };
        }
    }
    return map;
}

int MapDraw(Map* m) {
    // Рисуем фоновую сетку
    for (int x = 0; x < MAP_WIDTH*CELL_SIZE+1; x += CELL_SIZE) {
        DrawLine(x, 0, x, MAP_WIDTH*CELL_SIZE, LIGHTGRAY);
    }
    for (int y = 0; y < MAP_HEIGHT*CELL_SIZE+1; y += CELL_SIZE) {
        DrawLine(0, y, MAP_HEIGHT*CELL_SIZE, y, LIGHTGRAY);
    }

    for (int j = 0; j < MAP_HEIGHT; j++) {
        for (int i = 0; i < MAP_WIDTH; i++) {
            switch (m->Grid[j][i].type) {
                case Tile_Void:
                    DrawRectangleLines(
                        i*CELL_SIZE+10,
                        j*CELL_SIZE+10,
                        CELL_SIZE-20,
                        CELL_SIZE-20,
                        DARKPURPLE
                    );
                    break;

                case Tile_Free: break;

                case Tile_Stone:
                    DrawRectangle(
                        i*CELL_SIZE+10,
                        j*CELL_SIZE+10,
                        CELL_SIZE-20,
                        CELL_SIZE-20,
                        DARKGRAY
                    );
                    break;

                case Tile_Berry:
                    DrawCircle(
                        i*CELL_SIZE+CELL_SIZE/2,
                        j*CELL_SIZE+CELL_SIZE/2,
                        8, RED
                    );
                    break;
            }
        }
    }

    return 0;
}

int MapCellIsFree(Map* m, int x, int y) {
    if (x < 0 || x >= MAP_WIDTH ||
        y < 0 || y >= MAP_HEIGHT)
        return 0;

    return (!m->Grid[y][x].owner && m->Grid[y][x].type != Tile_Stone);
}
