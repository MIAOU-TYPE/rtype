/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BossSystem
*/

#include "BossSystem.hpp"
#include <iostream>

namespace Game
{
    void BossSystem::update(IGameWorld &world, float dt) {
        auto &reg = world.registry();
        (void) dt;

        reg.view<Ecs::BossPhase, Ecs::Health, Ecs::Position, Ecs::MovementPattern, Ecs::Drawable>(
            [&](const Ecs::Entity e, Ecs::BossPhase &bossPhase, const Ecs::Health &health,
                const Ecs::Position &pos, Ecs::MovementPattern &pattern, Ecs::Drawable &drawable) {
                if (pos.x < 800.f)
                    pattern.baseVx = 0.f;

                if (!bossPhase.phases.empty() && bossPhase.currentPhaseIndex + 1 < bossPhase.phases.size()) {
                    const auto &nextPhase = bossPhase.phases[bossPhase.currentPhaseIndex + 1];
                    if (health.hp <= nextPhase.healthThreshold) {
                        bossPhase.currentPhaseIndex++;
                        const auto &currentPhase = bossPhase.phases[bossPhase.currentPhaseIndex];
                        auto &attack = reg.getComponents<Ecs::AIShoot>();
                        if (!currentPhase.anglesToAdd.empty())
                            for (float angle : currentPhase.anglesToAdd) 
                                attack.at(static_cast<size_t>(e))->angles.push_back(angle);
                        if (currentPhase.fireSpeedMultiplier > 0.f)
                            attack.at(static_cast<size_t>(e))->projectileSpeed *= currentPhase.fireSpeedMultiplier;
                        if (currentPhase.damageMultiplier > 0)
                            attack.at(static_cast<size_t>(e))->damage =
                                static_cast<int>(attack.at(static_cast<size_t>(e))->damage * currentPhase.damageMultiplier);
                        if (currentPhase.spriteId > 0) {
                            drawable.spriteId = currentPhase.spriteId;
                        }
                    }
                }
            });
    }
} // namespace Game