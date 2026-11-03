#pragma once
#include "raylib.h"
struct WorldInput {
    Vector2 move{};
    bool spawnDebugEnemy = false;
    bool killAllEnemies = false;
    int debugDamage = 0;
};