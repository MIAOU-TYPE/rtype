/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

namespace Game
{
    static bool intersects(
        const Ecs::Position &a, const Ecs::Collision &ac, const Ecs::Position &b, const Ecs::Collision &bc)
    {
        return !(a.x > b.x + bc.width || a.x + ac.width < b.x || a.y > b.y + bc.height || a.y + ac.height < b.y);
    }

    void CollisionSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();

        auto &posArr = reg.getComponents<Ecs::Position>();
        auto &colArr = reg.getComponents<Ecs::Collision>();
        auto &dmgArr = reg.getComponents<Ecs::Damage>();
        auto &hpArr = reg.getComponents<Ecs::Health>();

        std::vector<Ecs::Entity> toDestroy;

        for (size_t i = 0; i < posArr.size(); i++) {
            if (!posArr.at(i) || !colArr.at(i))
                continue;

            for (size_t j = i + 1; j < posArr.size(); j++) {
                if (!posArr.at(j) || !colArr.at(j))
                    continue;
                if (!intersects(*posArr.at(i), *colArr.at(i), *posArr.at(j), *colArr.at(j)))
                    continue;
                if (dmgArr.at(i) && hpArr.at(j)) {
                    hpArr.at(j)->hp -= dmgArr.at(i)->amount;
                    toDestroy.emplace_back(i);
                } else if (dmgArr.at(j) && hpArr.at(i)) {
                    hpArr.at(i)->hp -= dmgArr.at(j)->amount;
                    toDestroy.emplace_back(j);
                }
            }
        }

        for (auto ent : toDestroy)
            reg.destroyEntity(ent);
    }
} // namespace Game
