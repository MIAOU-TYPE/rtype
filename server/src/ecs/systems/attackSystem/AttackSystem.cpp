/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AttackSystem
*/

#include "AttackSystem.hpp"
#include "AIBrain.hpp"
#include "Attack.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Position.hpp"
#include "Target.hpp"
#include "Velocity.hpp"

#include <cmath>

namespace Game
{
    void AttackSystem::update(IGameWorld &world, float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::AIBrain, Ecs::Target, Ecs::Attack, Ecs::Position>(
            [&](Ecs::Entity, Ecs::AIBrain &brain, const Ecs::Target &target, Ecs::Attack &atk, Ecs::Position &pos) {
                if (brain.state != Ecs::AIState::Attack)
                    return;

                if (brain.attackCooldown > 0.f) {
                    brain.attackCooldown -= dt;
                    return;
                }
                if (target.targetId == SIZE_MAX)
                    return;
                auto &posArr = reg.getComponents<Ecs::Position>();
                if (!posArr.at(target.targetId))
                    return;

                auto [x, y] = *posArr.at(target.targetId);

                float dx = x - pos.x;
                float dy = y - pos.y;
                const float len = std::sqrt(dx * dx + dy * dy);
                if (len == 0)
                    return;

                dx /= len;
                dy /= len;

                const Ecs::Entity proj = reg.createEntity();
                reg.emplaceComponent<Ecs::Position>(proj, pos.x, pos.y);
                reg.emplaceComponent<Ecs::Velocity>(proj, dx * atk.projectileSpeed, dy * atk.projectileSpeed);
                reg.emplaceComponent<Ecs::Damage>(proj, atk.damage);
                reg.emplaceComponent<Ecs::Collision>(proj, 8.f, 8.f);
                brain.attackCooldown = atk.cooldown;
            });
    }
} // namespace Game
