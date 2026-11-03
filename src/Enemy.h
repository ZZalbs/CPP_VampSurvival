#pragma once
#include "raylib.h"
class Enemy {
public:
    explicit Enemy(Vector2 position);
    void Update(float dt, Vector2 target);
    void TakeDamage(int damage);
    void Draw() const;
    Vector2 GetPosition() const;
    float GetRadius() const;
    int GetContactDamage() const;
    bool IsDead() const;
private:
    Vector2 position_{};
    int hp_ = 30;
    float speed_ = 70.0f;
    float radius_ = 12.0f;
    int contactDamage_ = 10;
};