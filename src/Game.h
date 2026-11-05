#pragma once
#include "World.h"

class Game
{
public:
    void Update(float dt) {
        if (state_ == GameState::Title || state_ == GameState::GameOver) {
            if (IsKeyPressed(KEY_ENTER)) StartRun();
            return;
        }

        if (state_ == GameState::LevelUp) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                const Vector2 mouse = GetMousePosition();
                for (const auto& option : UpgradeOptions) {
                    if (!CheckCollisionPointRec(mouse, option.bounds)) continue;
                    state_ = GameState::Playing;
                    CheckLevelUp();
                    break;
                }
            }
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
        else CheckLevelUp();
    }

    void Draw() const {
        ClearBackground(Color{ 18, 22, 30, 255 });
        if (state_ == GameState::Title) {
            DrawText("SHAPE SURVIVOR", 420, 260, 40, SKYBLUE);
            DrawText("ENTER: Start", 490, 330, 28, WHITE);
            DrawText("WASD / Arrows: Move", 420, 380, 24, LIGHTGRAY);
        }
        else
        {
            world_.Draw();
            DrawText(TextFormat("HP %d | Lv %d | XP %d/%d | Time %.1f",
                world_.GetHp(), level_, world_.GetExperience(), nextLevelXp_, elapsed_), 20, 20, 24, WHITE);
            if (state_ == GameState::LevelUp) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.75f));
                DrawText("LEVEL UP", 530, 160, 40, WHITE);
                DrawText("Choose an option", 455, 212, 26, LIGHTGRAY);
                const Vector2 mouse = GetMousePosition();
                for (const auto& option : UpgradeOptions) {
                    const bool hovered = CheckCollisionPointRec(mouse, option.bounds);
                    DrawRectangleRec(option.bounds, hovered ? DARKBLUE : DARKGRAY);
                    DrawText(option.label, static_cast<int>(option.bounds.x) + 16,
                             static_cast<int>(option.bounds.y) + 14, 24, WHITE);
                }
            }
            else if (state_ == GameState::GameOver) {
                DrawText("GAME OVER", 490, 270, 42, RED);
                DrawText("ENTER: Restart", 470, 330, 28, WHITE);
            }
        }
        DrawText("ESC: Quit", 20, 680, 20, GRAY);
    }

private:
    enum class GameState { Title, Playing, LevelUp, GameOver };
    struct UpgradeOption
    {
        Rectangle bounds;
        const char* label;
    };
    static constexpr UpgradeOption UpgradeOptions[] = {
        {{420, 260, 440, 52}, "Option 1 (no effect)"},
        {{420, 324, 440, 52}, "Option 2 (no effect)"},
        {{420, 388, 440, 52}, "Option 3 (no effect)"}
    };

    void CheckLevelUp() {
        if (world_.GetExperience() < nextLevelXp_) return;
        ++level_;
        nextLevelXp_ += 5;
        state_ = GameState::LevelUp;
    }

    void StartRun() {
        world_.Reset();
        elapsed_ = 0.0f;
        level_ = 1;
        nextLevelXp_ = 5;
        state_ = GameState::Playing;
    }

    World world_;
    GameState state_ = GameState::Title;
    float elapsed_ = 0.0f;
    int level_ = 1;
    int nextLevelXp_ = 5;
};