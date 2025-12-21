/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeSystem
*/

#include "LifetimeSystem.hpp"
#include "Lifetime.hpp"

namespace Game
{
    void LifetimeSystem::update(IGameWorld &world, float dt)
    {
        auto &reg = world.registry();
        auto &lifeArr = reg.getComponents<Ecs::Lifetime>();

        for (size_t i = 0; i < lifeArr.size(); i++) {
            if (!lifeArr.at(i).has_value())
                continue;

            lifeArr.at(i)->remaining -= dt;
            if (lifeArr.at(i)->remaining <= 0.f)
                reg.destroyEntity(Ecs::Entity(i));
        }
    }
} // namespace Game
