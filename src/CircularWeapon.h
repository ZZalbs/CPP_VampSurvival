#pragma once
#include "WeaponStrategy.h"

class CircularWeapon final : public WeaponStrategy {
public:
    void Reset() override {
        remaining_=0;
        flashRemaining_=0;
    }

    void Update(float dt, const Player& player, WeaponContext& context) override {
        flashRemaining_=std::max(0.0f,flashRemaining_-dt);
        remaining_=std::max(0.0f,remaining_-dt);
        if(remaining_>0) return;

        const Vector2 origin=player.GetPosition();
        const auto& enemies=context.Enemies();
        bool hit=false;
        for(std::size_t i=0;i<enemies.Slots().size();++i) {
            const auto& slot=enemies.Slots()[i];
            if(!slot) continue;
            const auto& enemy=*slot;
            if(enemy.IsDead() || !Overlap(origin,Radius,enemy.GetPosition(),enemy.GetRadius())) continue;
            context.ApplyWeaponDamage(i,20);
            hit=true;
        }
        if(hit) {
            remaining_=2.0f;
            flashOrigin_=origin;
            flashRemaining_=0.2f;
        }
    }

    void Draw() const override {
        if(flashRemaining_<=0) return;
        DrawCircleV(flashOrigin_,Radius,Fade(SKYBLUE,0.22f));
        DrawCircleLinesV(flashOrigin_,Radius,SKYBLUE);
    }
private:
    static constexpr float Radius=80;
    float remaining_=0;
    float flashRemaining_=0;
    Vector2 flashOrigin_{};
};
