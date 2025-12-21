/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** DamageSystem
*/

#include "DamageSystem.hpp"

namespace Game
{
    void DamageSystem::update(IGameWorld &world)
    {
        auto &reg = world.registry();
        auto &hpArr = reg.getComponents<Ecs::Health>();

        for (size_t i = 0; i < hpArr.size(); i++) {
            if (hpArr.at(i).has_value() && hpArr.at(i)->hp <= 0) {
                reg.destroyEntity(Ecs::Entity(i));
            }
        }
    }
} // namespace Game
