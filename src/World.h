#pragma once
#include "Collision.h"
#include "SpawnSystem.h"
#include <vector>
#include <random>
#include "StraightWeapon.h"
#include "CircularWeapon.h"
#include <array>
#include <memory>
#include <cstddef>
struct HitResult { std::size_t enemyIndex; float fraction; };

class World : private WeaponContext {
public:

    void Reset() {
        player_.Reset(); spawn_.Reset();
        for(auto& weapon:weapons_) weapon->Reset();
        enemies_.clear(); projectiles_.clear(); orbs_.clear();
        enemies_.reserve(300); projectiles_.reserve(256); orbs_.reserve(500);
    }

    void Update(float dt,Vector2 input,bool debugDamage,bool debugSpawn,bool debugKill) {
        if(dt<=0) return;
        player_.UpdateTimers(dt); player_.Move(input,dt);
        if(spawn_.Tick(dt)) SpawnEnemy();
        if(debugSpawn) SpawnEnemy();
        if(debugKill) for(auto& enemy:enemies_) enemy.DebugKill();
        for(auto& enemy:enemies_) enemy.Update(player_.GetPosition(),dt);
        UpdateProjectiles(dt);
        RemoveSpentProjectiles();
        for(auto& weapon:weapons_) weapon->Update(dt,player_,*this);
        CollectExperience();
        int damage=debugDamage ? 25 : 0;
        for(const auto& enemy:enemies_)
            if(!enemy.IsDead() && Overlap(player_.GetPosition(),player_.GetRadius(),
                                         enemy.GetPosition(),enemy.GetRadius()))
                damage=std::max(damage,10);
        player_.TryTakeDamage(damage);
        enemies_.erase(std::remove_if(enemies_.begin(),enemies_.end(),
                      [](const Enemy& e){return e.IsDead();}),enemies_.end());
    }

    int GetHp() const {
        return player_.GetHp();
    }

    int GetExperience() const {
        return player_.GetExperience();
    }

    std::size_t EnemyCount() const {
        return enemies_.size();
    }

    std::size_t ProjectileCount() const {
        return projectiles_.size();
    }

    std::size_t OrbCount() const {
        return orbs_.size();
    }

    int DroppedExperience() const {
        int sum=0;
        for(const auto& o:orbs_) sum+=o.value;
        return sum;
    }

    void Draw() const {
        for(const auto& o:orbs_) DrawRectangle(static_cast<int>(o.position.x)-4,static_cast<int>(o.position.y)-4,8,8,GREEN);
        for(const auto& weapon:weapons_) weapon->Draw();
        for(const auto& e:enemies_) e.Draw();
        for(const auto& p:projectiles_) p.Draw();
        player_.Draw();
    }

private:

    void SpawnEnemy() {
        if(enemies_.size()<300) enemies_.emplace_back(spawn_.NextPosition());
    }

    const std::vector<Enemy>& Enemies() const override {
        return enemies_;
    }

    bool TrySpawnProjectile(const ShotRequest& shot) override {
        if(projectiles_.size()>=256) return false;
        projectiles_.emplace_back(shot); return true;
    }

    std::optional<HitResult> FindFirstHit(const Projectile& p,float fraction) const {
        std::optional<HitResult> best;
        for(std::size_t i=0;i<enemies_.size();++i) {
            const auto& e=enemies_[i];
            if(e.IsDead()) continue;
            Vector2 end=Add(e.GetPreviousPosition(), Scale(Sub(e.GetPosition(),e.GetPreviousPosition()),fraction));
            auto t=FirstRelativeCircleHit(p.GetPreviousPosition(),p.GetPosition(), e.GetPreviousPosition(),end,p.GetRadius()+e.GetRadius());
            if(t && (!best || *t<best->fraction)) best=HitResult{i,*t};
        }
        return best;
    }

    void UpdateProjectiles(float dt) {
        for(auto& p:projectiles_) {
            float used=p.Advance(dt);
            if(used<=0) continue;
            auto hit=FindFirstHit(p,used/dt);
            if(hit) {
                ApplyWeaponDamage(hit->enemyIndex,p.GetDamage());
                p.MarkHit();
            }
        }
    }

    void ApplyWeaponDamage(std::size_t index,int damage) override {
        auto& e=enemies_.at(index);
        if(e.ApplyDamage(damage)==DamageResult::Killed) DropExperience(e.GetPosition(),1);
    }

    void DropExperience(Vector2 position,int value) {
        if(orbs_.size()<500) orbs_.push_back({position,value});
        else orbs_.back().value+=value;
    }

    void CollectExperience() {
        const Vector2 position=player_.GetPosition();
        const float radius=player_.GetRadius();
        orbs_.erase(std::remove_if(orbs_.begin(),orbs_.end(),
            [this,position,radius](const ExperienceOrb& orb) {
                if(!Overlap(position,radius,orb.position,4.0f)) return false;
                player_.AddExperience(orb.value);
                return true;
            }),orbs_.end());
    }

    void RemoveSpentProjectiles() {
        projectiles_.erase(std::remove_if(projectiles_.begin(),projectiles_.end(), [](const Projectile& p){return p.ShouldRemove();}),projectiles_.end());
    }

    Player player_;
    SpawnSystem spawn_;
    std::array<std::unique_ptr<WeaponStrategy>,2> weapons_{
        std::make_unique<StraightWeapon>(), std::make_unique<CircularWeapon>()
    };
    std::vector<Enemy> enemies_;
    std::vector<Projectile> projectiles_;
    std::vector<ExperienceOrb> orbs_;
};