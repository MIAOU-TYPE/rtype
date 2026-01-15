/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GravitySystem
*/

#include "GravitySystem.hpp"

namespace Game
{
    void GravitySystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        const float dampingFactor = 0.95f;
        reg.view<Ecs::Velocity, Ecs::GravityAffected>(
            [&](const Ecs::Entity, Ecs::Velocity &vel, Ecs::GravityAffected) {
                vel.vx *= dampingFactor;
                vel.vy *= dampingFactor;
            });

        reg.view<Ecs::Position, Ecs::GravityField, Ecs::Id>(
            [&](const Ecs::Entity, const Ecs::Position &gravPos, Ecs::GravityField &field, const Ecs::Id &id) {
                field.lifetime -= dt;
                if (field.lifetime <= 0.0f) {
                    world.events().emit(DestroyEvent{id.id, false});
                    return;
                }
                reg.view<Ecs::Position, Ecs::Velocity, Ecs::Health, Ecs::GravityAffected, Ecs::Id>(
                    [&](const Ecs::Entity, Ecs::Position &pos, Ecs::Velocity &vel, const Ecs::Health &hp,
                        Ecs::GravityAffected, const Ecs::Id &targetId) {
                        if (hp.hp <= 0)
                            return;

                        const float dx = gravPos.x - pos.x;
                        const float dy = gravPos.y - pos.y;
                        const float distance = std::sqrt(dx * dx + dy * dy);

                        if (distance >= field.radius || distance < 0.1f)
                            return;

                        const float pullFactor = field.pullStrength * dt / distance;
                        vel.vx += dx * pullFactor;
                        vel.vy += dy * pullFactor;

                        if (distance < field.innerRadius) {
                            const int damage = static_cast<int>(field.damagePerSecond * dt);
                            if (damage > 0)
                                world.events().emit(DamageEvent{id.id, targetId.id, damage});
                        }
                    });
            });
    }
} // namespace Game
