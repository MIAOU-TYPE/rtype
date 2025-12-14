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

    void Registry::destroyEntity(const Entity entity) noexcept
    {
        try {
            for (auto &func : _destroyers)
                func(*this, entity);
        } catch (...) {
            return;
        }
    }

    void Registry::clear() noexcept
    {
        _entityCounter = 0;
        _entityToIndex.clear();
        _destroyers.clear();
    }
} // namespace Ecs
