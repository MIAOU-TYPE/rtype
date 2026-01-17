/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** TailFollowerSystem
*/

#include "TailFollowerSystem.hpp"

namespace Game
{
    void TailFollowerSystem::update(IGameWorld &world, float dt)
    {
        auto &reg = world.registry();
        float waveAmplitude = 30.f;
        float waveFrequency = 3.f;
        static float time = 0.f;
        time += dt;

        reg.view<Ecs::TailFollower, Ecs::Position, Ecs::Velocity>(
            [&](Ecs::Entity entity, Ecs::TailFollower &tailFollower, Ecs::Position &position, Ecs::Velocity &velocity) {
                auto &healthArr = reg.getComponents<Ecs::Health>();
                const auto &bossHealth = healthArr.at(static_cast<size_t>(tailFollower.bossEntity));
                if (!bossHealth || bossHealth->hp <= 0) {
                    world.events().emit(DestroyEvent{static_cast<size_t>(entity), false});
                    return;
                }
                if (!reg.hasComponent<Ecs::Position>(tailFollower.leaderEntity))
                    return;
                const auto &bossPosOpt =
                    reg.getComponents<Ecs::Position>().at(static_cast<size_t>(tailFollower.bossEntity));
                if (!bossPosOpt.has_value())
                    return;
                const auto &bossPos = bossPosOpt.value();
                if (bossPos.x <= 800.f)
                    velocity.vx = 0.f;

                const auto &leaderPosition =
                reg.getComponents<Ecs::Position>().at(static_cast<size_t>(tailFollower.leaderEntity)).value();
                float targetY = 0.f;
                if (tailFollower.segmentIndex == 0) 
                    targetY = leaderPosition.y + 300.f + waveAmplitude * std::sin(waveFrequency * time + static_cast<float>(tailFollower.segmentIndex));
                else
                    targetY = leaderPosition.y + waveAmplitude * std::sin(waveFrequency * time + static_cast<float>(tailFollower.segmentIndex));
                
                float baseOffsetX = 0.f;
                if (tailFollower.segmentIndex == 0)
                    baseOffsetX = 55.f;
                else
                    baseOffsetX = -30.f;
                float dx = (leaderPosition.x + baseOffsetX) - position.x;
                float dy = targetY - position.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance > 1.f) {
                    dx /= distance;
                    dy /= distance;
                    velocity.vx = dx * tailFollower.followSpeed;
                    velocity.vy = dy * tailFollower.followSpeed;
                } else {
                    velocity.vx = 0.f;
                    velocity.vy = 0.f;
                }
            });
    }
} // namespace Game