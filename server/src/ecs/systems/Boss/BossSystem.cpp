/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BossSystem
*/

#include "BossSystem.hpp"

namespace Game
{
    void BossSystem::update(IGameWorld &world, float dt)
    {
        auto &reg = world.registry();
        (void)dt;

        reg.view<Ecs::BossPhase, Ecs::Health, Ecs::Id, Ecs::Position, Ecs::MovementPattern>(
            [&](const Ecs::Entity e, Ecs::BossPhase &bossPhase, const Ecs::Health &health, const Ecs::Id, const Ecs::Position &pos, Ecs::MovementPattern &pattern) {
                int currentThreshold = 0;
                switch (bossPhase.currentPhase) {
                    case Ecs::BossPhase::Phase::Phase1: currentThreshold = bossPhase.phaseThresholds[0]; break;
                    case Ecs::BossPhase::Phase::Phase2: currentThreshold = bossPhase.phaseThresholds[1]; break;
                    case Ecs::BossPhase::Phase::Phase3: currentThreshold = bossPhase.phaseThresholds[2]; break;
                }

                if (pos.x < 800.f)
                    pattern.baseVx = 0.f;

                if (health.hp <= currentThreshold) {
                    auto &attack = reg.getComponents<Ecs::AIShoot>();
                    if (bossPhase.currentPhase == Ecs::BossPhase::Phase::Phase1) {
                        bossPhase.currentPhase = Ecs::BossPhase::Phase::Phase2;
                        attack.at(static_cast<size_t>(e))->angles.push_back(-67.5f);
                        attack.at(static_cast<size_t>(e))->angles.push_back(-22.5f);
                        attack.at(static_cast<size_t>(e))->angles.push_back(-292.5f);
                    } else if (bossPhase.currentPhase == Ecs::BossPhase::Phase::Phase2) {
                        bossPhase.currentPhase = Ecs::BossPhase::Phase::Phase3;
                        attack.at(static_cast<size_t>(e))->angles.push_back(-78.75f);
                        attack.at(static_cast<size_t>(e))->angles.push_back(-56.25f);
                        attack.at(static_cast<size_t>(e))->angles.push_back(-11.25f);
                    }
                    attack.at(static_cast<size_t>(e))->projectileSpeed *= bossPhase.fireSpeedMultiplier;
                    attack.at(static_cast<size_t>(e))->damage =
                        static_cast<int>(attack.at(static_cast<size_t>(e))->damage * bossPhase.damageMultiplier);
                    
                }
            });
    }
} // namespace Game