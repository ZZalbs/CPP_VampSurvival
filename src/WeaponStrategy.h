#pragma once
#include "Projectile.h"
#include <cstddef>
#include <vector>

class WeaponContext {
public:
    virtual ~WeaponContext() = default;
    virtual const std::vector<Enemy>& Enemies() const = 0;
    virtual bool TrySpawnProjectile(const ShotRequest& shot) = 0;
    virtual void ApplyWeaponDamage(std::size_t index, int damage) = 0;
};

class WeaponStrategy {
public:
    virtual ~WeaponStrategy() = default;
    virtual void Reset() = 0;
    virtual void Update(float dt, const Player& player, WeaponContext& context) = 0;
    virtual void Draw() const {
    }
};
