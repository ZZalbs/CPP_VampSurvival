#pragma once
#include "Player.h"

struct ShotRequest {
    Vector2 position{},velocity{};
    int damage=15;
    float radius=4,lifetime=2;
};
class Projectile {
public:
    explicit Projectile(const ShotRequest& s): previous_(s.position),position_(s.position),
        velocity_(s.velocity),damage_(s.damage),radius_(s.radius),remaining_(s.lifetime) {
    }

    float Advance(float dt) {
        previous_=position_;
        float used=std::min(dt,std::max(0.0f,remaining_));
        position_=Add(position_,Scale(velocity_,used));
        remaining_=std::max(0.0f,remaining_-used); return used;
    }

    void MarkHit() {
        hit_=true;
    }

    bool ShouldRemove() const {
        return hit_ || remaining_<=0;
    }

    Vector2 GetPreviousPosition() const {
        return previous_;
    }

    Vector2 GetPosition() const {
        return position_;
    }

    float GetRadius() const {
        return radius_;
    }

    int GetDamage() const {
        return damage_;
    }

    void Draw() const {
        DrawCircleV(position_,radius_,YELLOW);
    }
private:
    Vector2 previous_,position_,velocity_;
    int damage_;
    float radius_,remaining_;
    bool hit_=false;
};
