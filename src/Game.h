#pragma once
#include "World.h"

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

        const bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
        const bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
        const bool down = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
        const bool up = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
        const Vector2 input = {
            static_cast<float>(right) - static_cast<float>(left),
            static_cast<float>(down) - static_cast<float>(up)
        };
        world_.Update(dt, input, IsKeyPressed(KEY_F1), IsKeyPressed(KEY_F5), IsKeyPressed(KEY_F6));
        elapsed_ += dt;
        if (world_.GetHp() == 0) state_ = GameState::GameOver;
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
            world_.Draw();
            DrawText(TextFormat("HP %d | Time %.1f",
                world_.GetHp(), elapsed_), 20, 20, 24, WHITE);
            if (state_ == GameState::GameOver)
            {
                DrawText("GAME OVER", 490, 270, 42, RED);
                DrawText("ENTER: Restart", 470, 330, 28, WHITE);
            }
        }
        DrawText("ESC: Quit", 20, 680, 20, GRAY);
    }

private:
    enum class GameState { Title, Playing, GameOver };
    void StartRun()
    {
        world_.Reset();
        elapsed_ = 0.0f;
        state_ = GameState::Playing;
    }

    World world_;
    GameState state_ = GameState::Title;
    float elapsed_ = 0.0f;
};