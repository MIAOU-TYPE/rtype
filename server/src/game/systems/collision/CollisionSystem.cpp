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
            if (!posArr[i].has_value() || !colArr[i].has_value())
                continue;

            for (size_t j = i + 1; j < posArr.size(); j++) {
                if (!posArr[j].has_value() || !colArr[j].has_value())
                    continue;
                if (!intersects(*posArr[i], *colArr[i], *posArr[j], *colArr[j]))
                    continue;
                if (dmgArr[i].has_value() && hpArr[j].has_value()) {
                    hpArr[j]->hp -= dmgArr[i]->amount;
                    toDestroy.emplace_back(Ecs::Entity(i));
                }
                if (dmgArr[j].has_value() && hpArr[i].has_value()) {
                    hpArr[i]->hp -= dmgArr[j]->amount;
                    toDestroy.emplace_back(Ecs::Entity(j));
                }
            }
        }

        for (auto ent : toDestroy)
            reg.destroyEntity(ent);
    }
} // namespace Game
