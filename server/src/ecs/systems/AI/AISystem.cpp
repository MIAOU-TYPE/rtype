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

        reg.view<Ecs::AIBrain, Ecs::Target, Ecs::Velocity, Ecs::Position>(
            [&](Ecs::Entity, Ecs::AIBrain &brain, const Ecs::Target &tgt, Ecs::Velocity &vel,
                const Ecs::Position &pos) {
                brain.timer += dt;

                const bool hasTarget = (tgt.targetId != SIZE_MAX);
                switch (brain.state) {
                    case Ecs::AIState::Idle:
                        vel.vx = -20.f;
                        vel.vy = 0.f;
                        if (hasTarget)
                            brain.state = Ecs::AIState::Chase;
                        else if (brain.timer > 1.5f) {
                            brain.timer = 0.f;
                            brain.state = Ecs::AIState::Patrol;
                        }
                        break;

                    case Ecs::AIState::Patrol:
                        vel.vx = -40.f + Rand::patrolVelX(Rand::rng);
                        vel.vy = Rand::patrolVelY(Rand::rng);

                        if (hasTarget)
                            brain.state = Ecs::AIState::Chase;
                        else if (brain.timer > 2.f) {
                            brain.timer = 0.f;
                            brain.state = Ecs::AIState::Idle;
                        }
                        break;

                    case Ecs::AIState::Chase:
                        if (!hasTarget) {
                            brain.state = Ecs::AIState::Patrol;
                            break;
                        }
                        {
                            auto &posArr = reg.getComponents<Ecs::Position>();
                            Ecs::Position playerPos = *posArr.at(tgt.targetId);

                            float dx = playerPos.x - pos.x;
                            float dy = playerPos.y - pos.y;
                            const float dist = std::sqrt(dx * dx + dy * dy);

                            dx /= (dist + 0.01f);
                            dy /= (dist + 0.01f);
                            vel.vx = dx * 80.f;
                            vel.vy = dy * 80.f;

                            if (dist < 200.f)
                                brain.state = Ecs::AIState::Attack;
                        }
                        break;

                    case Ecs::AIState::Attack:
                        vel.vx = -10.f;
                        vel.vy = 0.f;
                        if (!hasTarget)
                            brain.state = Ecs::AIState::Patrol;
                        break;
                    default: break;
                }
            });
    }
} // namespace Game
