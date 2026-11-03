#pragma once
#include "raylib.h"
class SpawnSystem {
public:
    void Reset();
    int ConsumeSpawnOpportunities(float dt);
    Vector2 ChoosePosition() const;
private:
    float elapsed_ = 0.0f;
    float interval_ = 1.0f;
};