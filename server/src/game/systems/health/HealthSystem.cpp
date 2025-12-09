/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HealthSystem
*/

#include "HealthSystem.hpp"

namespace Game
{
    void HealthSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();
        auto &hpArr = reg.getComponents<Ecs::Health>();

        for (size_t i = 0; i < hpArr.size(); i++) {
            if (!hpArr[i].has_value())
                continue;
            if (hpArr[i]->hp <= 0)
                reg.destroyEntity(Ecs::Entity(i));
        }
    }
} // namespace Game
