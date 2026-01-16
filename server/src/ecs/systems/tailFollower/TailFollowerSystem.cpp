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
        (void)dt;
        
        reg.view<Ecs::TailFollower, Ecs::Position, Ecs::Velocity>(
            [&](Ecs::Entity, Ecs::TailFollower &tailFollower, Ecs::Position &position, Ecs::Velocity &velocity) {

                if (!reg.hasComponent<Ecs::Position>(tailFollower.leaderEntity))
                    return;

                const auto &leaderPosition = reg.getComponents<Ecs::Position>().at(static_cast<size_t>(tailFollower.leaderEntity)).value();
                                                      
                float dx = leaderPosition.x - position.x;
                float dy = leaderPosition.y - position.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance > tailFollower.followDistance) {
                    if (distance > 0.f) {
                        dx /= distance;
                        dy /= distance;
                    }
                    velocity.vx = dx * tailFollower.followSpeed;
                    velocity.vy = dy * tailFollower.followSpeed;
                } else {
                    velocity.vx = 0.f;
                    velocity.vy = 0.f;
                }
            });
            
    }
} // namespace Game