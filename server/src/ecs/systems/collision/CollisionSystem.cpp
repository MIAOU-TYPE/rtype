/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

namespace
{
    [[nodiscard]] bool intersects(
        const Ecs::Position &a, const Ecs::Collision &ac, const Ecs::Position &b, const Ecs::Collision &bc)
    {
        return !(a.x > b.x + bc.width || a.x + ac.width < b.x || a.y > b.y + bc.height || a.y + ac.height < b.y);
    }

    [[nodiscard]] bool bothAI(const Ecs::Registry &reg, const size_t a, const size_t b)
    {
        return reg.hasComponent<Ecs::AIBrain>(Ecs::Entity(a)) && reg.hasComponent<Ecs::AIBrain>(Ecs::Entity(b));
    }

    [[nodiscard]] bool sameShooter(Ecs::Registry reg, const size_t a, const size_t b)
    {
        auto shootArr = reg.getComponents<Ecs::Projectile>();
        return shootArr.at(a) && shootArr.at(b) && shootArr.at(a)->shooter == shootArr.at(b)->shooter;
    }

    [[nodiscard]] bool projectileHitsShooter(Ecs::Registry &reg, const size_t projectileIdx, const size_t targetIdx)
    {
        const auto &projectile = reg.getComponents<Ecs::Projectile>().at(projectileIdx);
        return projectile && projectile->shooter == targetIdx;
    }
} // namespace

namespace Game
{

    void CollisionSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        auto &posArr = reg.getComponents<Ecs::Position>();
        auto &colArr = reg.getComponents<Ecs::Collision>();

        for (size_t i = 0; i < posArr.size(); i++) {
            const auto &posA = posArr.at(i);
            const auto &colA = colArr.at(i);
            if (!posA || !colA)
                continue;

            for (size_t j = i + 1; j < posArr.size(); j++) {
                const auto &posB = posArr.at(j);
                const auto &colB = colArr.at(j);
                if (!posB || !colB)
                    continue;
                if (!intersects(*posA, *colA, *posB, *colB))
                    continue;
                if (bothAI(reg, i, j))
                    continue;
                if (projectileHitsShooter(reg, i, j) || projectileHitsShooter(reg, j, i))
                    continue;
                if (sameShooter(reg, i, j))
                    continue;
                world.events().emit(CollisionEvent{i, j});
            }
        }
    }
} // namespace Game
