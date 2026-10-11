#pragma once

#include "map.h"

// Old: Person Errors
typedef enum {
    PEs_Good = 0,
    PEs_isMove = 1,
    PEs_isStop = 2,
    PEs_StoneAhead = 3,
} PersonResults;

typedef enum {
    Way_Up = 0,
    Way_Down = 1,
    Way_Left = 2,
    Way_Right = 3,
} Way;

typedef struct {
    float screenX;
    float screenY;
    int X;
    int Y;
    int health;
    int BerryCount;
    int DefeatEnemyCount;
    float Speed;
    Way way;
    char isMoving;
} Person;

Person PersonInit(int x, int y, float speed, Way);
int PersonDraw(Person*);
int PersonAttack(Person*, Map*);

/* ==== Handlers ==== */

PersonResults PersonMove(Person*, Way, Map*);
PersonResults PersonRotate(Person*, Way);
PersonResults PersonUpdate(Person*);
