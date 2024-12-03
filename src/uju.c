#include "uju.h"

int main (void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "uju raylib");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("uju", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}