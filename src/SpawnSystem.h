#pragma once
#include "raylib.h"
#include <random>
class SpawnSystem {
public:
    void Reset() {
        elapsed_ = 0;
    }

    bool Tick(float dt) {
        elapsed_ += dt;
        if (elapsed_ < 1) return false;
        elapsed_ -= 1; return true;
    }

    Vector2 NextPosition() {
        std::uniform_int_distribution<int> side(0, 3);
        std::uniform_real_distribution<float> x(0, 1280), y(0, 720);
        switch (side(random_)) {
        case 0: return { -30,y(random_) };
        case 1: return { 1310,y(random_) };
        case 2: return { x(random_),-30 };
        default: return { x(random_),750 };
        }
    }
private:
    float elapsed_ = 0;
    std::mt19937 random_{ std::random_device{}() };
};