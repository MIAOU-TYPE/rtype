/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementPatternSystem
*/

#include "MovementPatternSystem.hpp"

namespace Game
{
    void MovementPatternSystem::update(IGameWorld &world, const float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::MovementPattern, Ecs::Velocity>(
            [&](const Ecs::Entity, Ecs::MovementPattern &pattern, Ecs::Velocity &vel) {
                pattern.timer += dt;

                switch (pattern.type) {
                    case Ecs::MovementPattern::Type::Straight:
                        vel.vx = pattern.baseVx;
                        vel.vy = 0.f;
                        break;

                    case Ecs::MovementPattern::Type::ZigZag: {
                        vel.vx = pattern.baseVx;
                        const float omega = 2.f * 3.14159265358979323846f * pattern.frequency;
                        vel.vy = pattern.amplitude * omega * std::cos(omega * pattern.timer);
                        break;
                    }
                }
            });
    }
} // namespace Game