/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Entity
*/

#include "Entity.hpp"

namespace Ecs
{
    Entity::Entity(const size_t id) : _id(id)
    {
    }

    Entity::operator size_t() const noexcept
    {
        return _id;
    }
} // namespace Ecs
