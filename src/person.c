// #include <stdio.h>
#include <raylib.h>
#include <stddef.h>

#include "person.h"
#include "map.h"

// #define CELL_SIZE 40

Person PersonInit(int x, int y, float speed, Way way) {
    return (Person) {
        x * CELL_SIZE,
        y * CELL_SIZE,
        x, y,
        1, 0, 0, speed, way, 0
    };
}

int PersonDraw(Person* p) {
    if (p->health <= 0) return 1;
    Rectangle body = {
        p->screenX + 10, p->screenY + 10,
        CELL_SIZE - 20, CELL_SIZE - 20
    };

    // Coords forford line
    Vector2 startLine;
    Vector2 endLine;

    // init forward line
    switch (p->way) {
        case Way_Up:
            startLine.x = body.x;
            startLine.y = body.y;
            endLine.x = body.x + body.width;
            endLine.y = body.y;
            break;

        case Way_Down:
            startLine.x = body.x;
            startLine.y = body.y + body.height;
            endLine.x   = body.x + body.width;
            endLine.y   = body.y + body.height;
            break;

        case Way_Left:
            startLine.x = body.x;
            startLine.y = body.y;
            endLine.x   = body.x;
            endLine.y   = body.y + body.height;
            break;

        case Way_Right:
            startLine.x = body.x + body.width;
            startLine.y = body.y;
            endLine.x   = body.x + body.width;
            endLine.y   = body.y + body.height;
            break;
    }

    // Рисуем ОБВОДКУ текущей клетки, где находится перс
    // Толщина линии — 3 пикселя, чтобы было хорошо видно
    DrawRectangleLinesEx(
        (Rectangle) {
            p->X * CELL_SIZE, p->Y * CELL_SIZE,
            CELL_SIZE, CELL_SIZE
        }, 3.0f, RED
    );

    // Рисуем самого персонажа (просто круг для теста в его float координатах)
    // DrawCircle((int)p->X, (int)p->Y, 15, BLUE);
    DrawRectangleRec(body, BLUE);

    DrawLineEx(startLine, endLine, 3.0f, YELLOW);

    return 0;
}

int PersonAttack(Person *p, Map *m) {
    Person *enemy = NULL;
    Cell *nextCell = NULL;
    switch (p->way) {
        case Way_Up:
            if (p->Y <= 0) break;
            nextCell = &m->Grid[p->Y-1][p->X];
            break;

        case Way_Down:
            if (p->Y >= MAP_HEIGHT-1) break;
            nextCell = &m->Grid[p->Y+1][p->X];
            break;

        case Way_Left:
            if (p->X <= 0) break;
            nextCell = &m->Grid[p->Y][p->X-1];
            break;

        case Way_Right:
            if (p->X >= MAP_WIDTH-1) break;
            nextCell = &m->Grid[p->Y][p->X+1];
            break;
    };

    if (!nextCell) return 2;
    enemy = nextCell->owner;

    switch (nextCell->type) {
        case Tile_Berry:
            nextCell->type = Tile_Free;
            p->BerryCount++;
            break;

        default:
            break;
    }

    if (!enemy) return 1;

    // printf("Yeas\n");

    enemy->health = 0;
    m->Grid[enemy->Y][enemy->X].owner = NULL;
    p->DefeatEnemyCount++;
    return 0;
}

/* ==== Handlers ==== */

PersonResults PersonMove(Person* p, Way w, Map* map) {
    if (p->isMoving) return PEs_isMove;

    p->way = w;

    switch (w) {
        case Way_Up:
            if (!MapCellIsFree(map, p->X, p->Y-1))
                return PEs_StoneAhead;

            map->Grid[p->Y][p->X].owner = NULL;
            p->Y -= 1;
            map->Grid[p->Y][p->X].owner = p;
            break;

        case Way_Down:
            if (!MapCellIsFree(map, p->X, p->Y+1))
                return PEs_StoneAhead;

            map->Grid[p->Y][p->X].owner = NULL;
            p->Y += 1;
            map->Grid[p->Y][p->X].owner = p;
            break;

        case Way_Left:
            if (!MapCellIsFree(map, p->X-1, p->Y))
                return PEs_StoneAhead;

            map->Grid[p->Y][p->X].owner = NULL;
            p->X -= 1;
            map->Grid[p->Y][p->X].owner = p;
            break;

        case Way_Right:
            if (!MapCellIsFree(map, p->X+1, p->Y))
                return PEs_StoneAhead;

            map->Grid[p->Y][p->X].owner = NULL;
            p->X += 1;
            map->Grid[p->Y][p->X].owner = p;
            break;
    }

    p->isMoving = 1;

    return PEs_Good;
}

PersonResults PersonRotate(Person* p, Way w) {
    p->way = w;
    return 0;
}

PersonResults PersonUpdate(Person *p) {
    float deltaTime = GetFrameTime();

    int gridXScaled = p->X * CELL_SIZE;
    int gridYScaled = p->Y * CELL_SIZE;

    // if (p->X < 0) p->gridX -= CELL_SIZE;
    // if (p->Y < 0) p->gridY -= CELL_SIZE;

    if (!p->isMoving) return PEs_isStop;

    // Плавное движение по оси X
    if (p->screenX < gridXScaled) {
        p->screenX += p->Speed * deltaTime;
        if (p->screenX > gridXScaled) p->screenX = gridXScaled; // Защита от «перелета»
    } else if (p->screenX > gridXScaled) {
        p->screenX -= p->Speed * deltaTime;
        if (p->screenX < gridXScaled) p->screenX = gridXScaled;
    }

    // Плавное движение по оси Y
    if (p->screenY < gridYScaled) {
        p->screenY += p->Speed * deltaTime;
        if (p->screenY > gridYScaled) p->screenY = gridYScaled;
    } else if (p->screenY > gridYScaled) {
        p->screenY -= p->Speed * deltaTime;
        if (p->screenY < gridYScaled) p->screenY = gridYScaled;
    }

    // Если дошли точно до цели, останавливаемся
    if (p->screenX == gridXScaled && p->screenY == gridYScaled) {
        p->isMoving = 0;
    }

    return 0;
}
