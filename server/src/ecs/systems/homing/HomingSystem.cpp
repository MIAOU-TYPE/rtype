/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** HomingSystem
*/

#include "HomingSystem.hpp"

namespace Game
{
    void HomingSystem::update(IGameWorld &world, float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::HomingProjectile, Ecs::Position, Ecs::Velocity>(
            [&](const Ecs::Entity, Ecs::HomingProjectile &homing, Ecs::Position &pos, Ecs::Velocity &vel) {
                auto &targetPosComp = reg.getComponents<Ecs::Position>().at(homing.targetId);
                if (!targetPosComp)
                    return;

                const float dx = targetPosComp->x - pos.x;
                const float dy = targetPosComp->y - pos.y;
                const float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 0.1f)
                    return;

                const float targetVx = (dx / distance) * homing.maxSpeed;
                const float targetVy = (dy / distance) * homing.maxSpeed;

                vel.vx += (targetVx - vel.vx) * homing.homingStrength * dt;
                vel.vy += (targetVy - vel.vy) * homing.homingStrength * dt;

                const float currentSpeed = std::sqrt(vel.vx * vel.vx + vel.vy * vel.vy);
                if (currentSpeed > homing.maxSpeed) {
                    vel.vx = (vel.vx / currentSpeed) * homing.maxSpeed;
                    vel.vy = (vel.vy / currentSpeed) * homing.maxSpeed;
                }
            });
    }
} // namespace Game