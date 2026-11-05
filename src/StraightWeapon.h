#pragma once
#include "WeaponStrategy.h"
#include <limits>

class StraightWeapon final : public WeaponStrategy {
public:
    void Reset() override {
        remaining_=0;
    }

    void Update(float dt, const Player& player, WeaponContext& context) override {
        remaining_=std::max(0.0f,remaining_-dt);
        if(remaining_>0) return;

        const Vector2 origin=player.GetPosition();
        const Enemy* target=nullptr;
        float closest=std::numeric_limits<float>::max();
        for(const auto& slot:context.Enemies().Slots()) {
            if(!slot) continue;
            const auto& enemy=*slot;
            if(enemy.IsDead()) continue;
            const float distance=LengthSquared(Sub(enemy.GetPosition(),origin));
            if(distance<closest) {
                closest=distance;
                target=&enemy;
            }
        }
        if(!target) return;

        const Vector2 velocity=Scale(Direction(Sub(target->GetPosition(),origin),player.GetFacing()),500);
        if(context.TrySpawnProjectile({origin,velocity})) remaining_=0.6f;
    }
private:
    float remaining_=0;
};
