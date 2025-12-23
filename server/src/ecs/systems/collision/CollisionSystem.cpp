/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

namespace
{
    bool intersects(const Ecs::Position &a, const Ecs::Collision &ac, const Ecs::Position &b, const Ecs::Collision &bc)
    {
        return !(a.x > b.x + bc.width || a.x + ac.width < b.x || a.y > b.y + bc.height || a.y + ac.height < b.y);
    }
} // namespace

namespace Game
{

    void CollisionSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        auto &posArr = reg.getComponents<Ecs::Position>();
        auto &colArr = reg.getComponents<Ecs::Collision>();
        auto &dmgArr = reg.getComponents<Ecs::Damage>();
        auto &hpArr = reg.getComponents<Ecs::Health>();
        auto &iaBrainArr = reg.getComponents<Ecs::AIBrain>();
        auto &projectileArr = reg.getComponents<Ecs::Projectile>();

        for (size_t i = 0; i < posArr.size(); i++) {
            if (!posArr.at(i) || !colArr.at(i))
                continue;

            for (size_t j = i + 1; j < posArr.size(); j++) {
                if (iaBrainArr.at(j) && iaBrainArr.at(i))
                    continue;
                if (!posArr.at(j) || !colArr.at(j))
                    continue;
                if (projectileArr.at(i) && projectileArr.at(j))
                    continue;
                if (projectileArr.at(i) && projectileArr.at(i)->shooter == j)
                    continue;
                if (projectileArr.at(j) && projectileArr.at(j)->shooter == i)
                    continue;
                if (!intersects(*posArr.at(i), *colArr.at(i), *posArr.at(j), *colArr.at(j)))
                    continue;
                if (dmgArr.at(i) && hpArr.at(j)) {
                    hpArr.at(j)->hp -= dmgArr.at(i)->amount;
                }
                if (dmgArr.at(j) && hpArr.at(i)) {
                    hpArr.at(i)->hp -= dmgArr.at(j)->amount;
                }
            }
        }
    }
} // namespace Game
