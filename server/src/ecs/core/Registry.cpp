/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

#include "Registry.hpp"

namespace Ecs
{
    Entity Registry::createEntity() noexcept
    {
        Entity entity(_entityCounter);
        _entityCounter++;
        return entity;
    }

    void Registry::destroyEntity(Entity entity) noexcept
    {
        for (auto &func : _destroyers)
            func(*this, entity);
    }
}
