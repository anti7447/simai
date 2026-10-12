#include <raylib.h>
#include "person.h"
#include "map.h"

#define MAX_PERSONS 5
// #define CELL_SIZE 40.0f

int main(void) {
    // Настройки окна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib 2D Grid Movement");

    // Person p = PersonInit(3, 3, 200.0f, Way_Up);
    Person Perries[MAX_PERSONS];

    for (int i = 0; i < MAX_PERSONS; i++) {
        Perries[i] = PersonInit(i, 0, 200.0f, Way_Up);
    }

    Map map = MapInit();
    Person *p = Perries;
    Person *p2 = Perries + 1;
    int cp1 = 0, cp2 = 0;
    // Person *enimy = Perries+1;

    // map.Grid[p->Y][p->X].owner = p;

    for (int i = 0; i < MAX_PERSONS; i++) {
        map.Grid[Perries[i].Y][Perries[i].X].owner = Perries + i;
    }

    map.Grid[5][4].type = Tile_Stone;
    map.Grid[7][4].type = Tile_Berry;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 1. ЛОГИКА ДВИЖЕНИЯ (просто для теста, чтобы перс двигался плавно)
        // float deltaTime = GetFrameTime();
        if (IsKeyDown(KEY_D))     PersonMove(p, Way_Right, &map); // playerX += speed * deltaTime;
        if (IsKeyDown(KEY_A))     PersonMove(p, Way_Left,  &map); // playerX -= speed * deltaTime;
        if (IsKeyDown(KEY_S))     PersonMove(p, Way_Down,  &map); // playerY += speed * deltaTime;
        if (IsKeyDown(KEY_W))     PersonMove(p, Way_Up,    &map); // playerY -= speed * deltaTime;
        if (IsKeyDown(KEY_Q))     PersonRotate(p, Way_Left);
        if (IsKeyDown(KEY_E))     PersonRotate(p, Way_Right);
        if (IsKeyDown(KEY_SPACE)) PersonAttack(p, &map);

        if (IsKeyDown(KEY_RIGHT))         PersonMove(p2, Way_Right, &map); // playerX += speed * deltaTime;
        if (IsKeyDown(KEY_LEFT))          PersonMove(p2, Way_Left,  &map); // playerX -= speed * deltaTime;
        if (IsKeyDown(KEY_DOWN))          PersonMove(p2, Way_Down,  &map); // playerY += speed * deltaTime;
        if (IsKeyDown(KEY_UP))            PersonMove(p2, Way_Up,    &map); // playerY -= speed * deltaTime;
        if (IsKeyDown(KEY_PERIOD))        PersonRotate(p2, Way_Left);
        if (IsKeyDown(KEY_SLASH))         PersonRotate(p2, Way_Right);
        if (IsKeyDown(KEY_RIGHT_CONTROL)) PersonAttack(p2, &map);

        // PersonRotate(&p, Way_Up);
        // PersonUpdate(p);
        for (int i = 0; i < MAX_PERSONS; i++) {
            if (!Perries[i].health) {
                MapRegen(&map);
                cp1 += p->DefeatEnemyCount;
                cp2 += p2->DefeatEnemyCount;
                for (int i = 0; i < MAX_PERSONS; i++) {
                    Perries[i] = PersonInit(i, 0, 200.0f, Way_Up);
                    map.Grid[Perries[i].Y][Perries[i].X].owner = Perries + i;
                }
                break;
            }
            PersonUpdate(Perries+i);
        }

        // 3. ОТРИСОВКА
        BeginDrawing();
        ClearBackground(RAYWHITE);

        MapDraw(&map);

        // PersonDraw(p);
        for (int i = 0; i < MAX_PERSONS; i++) {
            PersonDraw(Perries + i);
        }

        // Выведем текст для отладки
        DrawText(TextFormat("Player Float: %.2f, %.2f", p->screenX, p->screenY), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Grid Cell: %d, %d", (int)(p->X), (int)(p->Y)), 10, 35, 20, DARKGRAY);
        DrawText(TextFormat("Berries: %d", p->BerryCount), 10, 60, 20, DARKGRAY);
        DrawText(TextFormat("Enemies: %d", p->DefeatEnemyCount), 10, 85, 20, DARKGRAY);

        DrawText(TextFormat("P1 x P2"),
            (screenWidth / 2) - 60, 10, 20, DARKGRAY);
        DrawText(TextFormat("%d   %d", cp1, cp2),
            (screenWidth / 2) - 60, 35, 20, DARKGRAY);
        DrawText(TextFormat("%d   %d (Berries)", p->BerryCount, p2->BerryCount),
            (screenWidth / 2) - 60, 60, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
