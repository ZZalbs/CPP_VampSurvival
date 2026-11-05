#include "raylib.h"
#include <algorithm>
#include <cmath>
#include "Game.h"

namespace Config
{
    constexpr int ScreenWidth = 1280;
    constexpr int ScreenHeight = 720;
    constexpr float PlayerRadius = 12.0f;
    constexpr float PlayerSpeed = 220.0f;
    constexpr int MaxHp = 100;
}

int main() {
    InitWindow(1280, 720, "");
    SetTargetFPS(60);
    Game game;
    while (!WindowShouldClose()) {
        game.Update(std::clamp(GetFrameTime(), 0.0f, 0.1f));
        BeginDrawing();
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
}