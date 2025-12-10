/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AISystem
*/

#include "AISystem.hpp"

namespace Game
{
    void AISystem::update(IGameWorld &world, float dt)
    {
        auto &reg = world.registry();

        reg.view<Ecs::AIBrain, Ecs::Velocity>([dt](Ecs::Entity, Ecs::AIBrain &brain, Ecs::Velocity &vel) {
            brain.timer += dt;

            switch (brain.state) {
                case Ecs::AIState::Idle:
                    vel.vx = -20.f;
                    if (brain.timer > 1.f) {
                        brain.timer = 0.f;
                        brain.state = Ecs::AIState::Patrol;
                    }
                    break;

                case Ecs::AIState::Patrol:
                    vel.vx = -40.f + Rand::patrolVelX(Rand::rng);
                    vel.vy = Rand::patrolVelY(Rand::rng);

                    if (brain.timer > 2.f) {
                        brain.timer = 0.f;
                        brain.state = Ecs::AIState::Idle;
                    }
                    break;

                default: break;
            }
        });
    }
} // namespace Game
