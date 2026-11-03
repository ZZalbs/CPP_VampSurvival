#pragma once
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <optional>

inline Vector2 Add(Vector2 a, Vector2 b) { return {a.x+b.x,a.y+b.y}; }
inline Vector2 Sub(Vector2 a, Vector2 b) { return {a.x-b.x,a.y-b.y}; }
inline Vector2 Scale(Vector2 a,float s) { return {a.x*s,a.y*s}; }
inline float LengthSquared(Vector2 a) { return a.x*a.x+a.y*a.y; }
inline Vector2 Direction(Vector2 v,Vector2 fallback={1,0}) {
    float n=LengthSquared(v);
    if(n>1e-12f) return Scale(v,1/std::sqrt(n));
    n=LengthSquared(fallback);
    return n>1e-12f ? Scale(fallback,1/std::sqrt(n)) : Vector2{1,0};
}
inline bool Overlap(Vector2 a,float ar,Vector2 b,float br) {
    return LengthSquared(Sub(a,b))<=(ar+br)*(ar+br);
}
class Player {
public:
    void Reset() { position_={640,360}; facing_={1,0}; hp_=100; invincible_=0; }
    void UpdateTimers(float dt) { invincible_=std::max(0.0f,invincible_-dt); }
    void Move(Vector2 input,float dt) {
        if(LengthSquared(input)>0) {
            facing_=Direction(input);
            position_=Add(position_,Scale(facing_,220*dt));
        }
        position_.x=std::clamp(position_.x,12.0f,1268.0f);
        position_.y=std::clamp(position_.y,12.0f,708.0f);
    }
    bool TryTakeDamage(int damage) {
        if(damage<=0 || hp_==0 || invincible_>0) return false;
        hp_=std::max(0,hp_-damage); invincible_=0.5f; return true;
    }
    Vector2 GetPosition() const { return position_; }
    Vector2 GetFacing() const { return facing_; }
    float GetRadius() const { return 12; }
    int GetHp() const { return hp_; }
    void Draw() const {
        DrawCircleV(position_,12,invincible_>0 ? WHITE : SKYBLUE);
        DrawLineEx(position_,Add(position_,Scale(facing_,22)),3,WHITE);
    }
private:
    Vector2 position_{640,360},facing_{1,0};
    int hp_=100;
    float invincible_=0;
};
enum class DamageResult { Ignored, Hit, Killed };
class Enemy {
public:
    explicit Enemy(Vector2 p): previous_(p),position_(p) {}
    void Update(Vector2 target,float dt) {
        previous_=position_;
        if(IsDead()) return;
        Vector2 delta=Sub(target,position_);
        float distance=std::sqrt(LengthSquared(delta));
        if(distance>0) position_=Add(position_,Scale(delta,std::min(70*dt,distance)/distance));
    }
    DamageResult ApplyDamage(int damage) {
        if(IsDead() || damage<=0) return DamageResult::Ignored;
        hp_=std::max(0,hp_-damage);
        return IsDead() ? DamageResult::Killed : DamageResult::Hit;
    }
    void DebugKill() { hp_=0; }
    bool IsDead() const { return hp_==0; }
    Vector2 GetPosition() const { return position_; }
    Vector2 GetPreviousPosition() const { return previous_; }
    float GetRadius() const { return 12; }
    void Draw() const { DrawCircleV(position_,12,RED); }
private:
    Vector2 previous_,position_;
    int hp_=30;
};
struct ShotRequest {
    Vector2 position{},velocity{};
    int damage=15;
    float radius=4,lifetime=2;
};
class StraightWeapon {
public:
    void Reset() { remaining_=0; }
    void Tick(float dt) { remaining_=std::max(0.0f,remaining_-dt); }
    std::optional<ShotRequest> BuildRequest(Vector2 origin,Vector2 facing,
                                            std::optional<Vector2> target) const {
        if(remaining_>0 || !target) return std::nullopt;
        return ShotRequest{origin,Scale(Direction(Sub(*target,origin),facing),500),15,4,2};
    }
    void CommitShot() { remaining_=0.6f; }
private:
    float remaining_=0;
};
class Projectile {
public:
    explicit Projectile(const ShotRequest& s): previous_(s.position),position_(s.position),
        velocity_(s.velocity),damage_(s.damage),radius_(s.radius),remaining_(s.lifetime) {}
    float Advance(float dt) {
        previous_=position_;
        float used=std::min(dt,std::max(0.0f,remaining_));
        position_=Add(position_,Scale(velocity_,used));
        remaining_=std::max(0.0f,remaining_-used); return used;
    }
    void MarkHit() { hit_=true; }
    bool ShouldRemove() const { return hit_ || remaining_<=0; }
    Vector2 GetPreviousPosition() const { return previous_; }
    Vector2 GetPosition() const { return position_; }
    float GetRadius() const { return radius_; }
    int GetDamage() const { return damage_; }
    void Draw() const { DrawCircleV(position_,radius_,YELLOW); }
private:
    Vector2 previous_,position_,velocity_;
    int damage_;
    float radius_,remaining_;
    bool hit_=false;
};
struct ExperienceOrb { Vector2 position; int value=1; };