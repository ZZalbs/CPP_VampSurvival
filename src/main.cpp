#include "raylib.h"
#include <algorithm>
#include <cmath>
#include "Player.h"

namespace Config
{
    constexpr int ScreenWidth = 1280;
    constexpr int ScreenHeight = 720;
    constexpr float PlayerRadius = 12.0f;
    constexpr float PlayerSpeed = 220.0f;
    constexpr int MaxHp = 100;
}


enum class GameState { Title, Playing, GameOver };


class Game
{
public:
    void Update(float dt)
    {
        if (state_ == GameState::Title || state_ == GameState::GameOver)
        {
            if (IsKeyPressed(KEY_ENTER)) StartRun();
            return;
        }

        elapsed_ += dt;
        const bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
        const bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
        const bool down = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
        const bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
        const Vector2 direction = {
            static_cast<float>(right) - static_cast<float>(left),
            static_cast<float>(down) - static_cast<float>(up)
        };
        player_.Move(direction, dt);

        player_.UpdateTimers(dt);
        if (IsKeyPressed(KEY_F1)) player_.TryTakeDamage(25);
        if (player_.GetHp() <= 0) state_ = GameState::GameOver;
    }

    void Draw() const
    {
        ClearBackground(Color{ 18, 22, 30, 255 });
        if (state_ == GameState::Title)
        {
            DrawText("SHAPE SURVIVOR", 420, 260, 40, SKYBLUE);
            DrawText("ENTER: Start", 490, 330, 28, WHITE);
            DrawText("WASD / Arrows: Move", 420, 380, 24, LIGHTGRAY);
        }
        else
        {
            player_.Draw();
            const Vector2 pos = player_.GetPosition();
            DrawText(TextFormat("HP: %d / %d", player_.GetHp(),
                Config::MaxHp), 20, 20, 24, WHITE);
            DrawText(TextFormat("Time: %.2f s", elapsed_),
                20, 52, 24, WHITE);
            DrawText(TextFormat("Position: (%.2f, %.2f)", pos.x, pos.y),
                20, 84, 22, LIGHTGRAY);
            DrawText("F1: Debug damage (-25)", 20, 116, 22, LIGHTGRAY);
            if (state_ == GameState::GameOver)
            {
                DrawText("GAME OVER", 490, 270, 42, RED);
                DrawText("ENTER: Restart", 470, 330, 28, WHITE);
            }
        }
        DrawText("ESC: Quit", 20, 680, 20, GRAY);
    }

private:
    void StartRun()
    {
        player_.Reset();
        elapsed_ = 0.0f;
        state_ = GameState::Playing;
    }

    Player player_;
    GameState state_ = GameState::Title;
    float elapsed_ = 0.0f;
};

int main()
{
    InitWindow(Config::ScreenWidth, Config::ScreenHeight,
        "Shape Survivor");
    SetTargetFPS(60);
    Game game;
    while (!WindowShouldClose())
    {
        const float dt = std::clamp(GetFrameTime(), 0.0f, 0.1f);
        game.Update(dt);
        BeginDrawing();
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}