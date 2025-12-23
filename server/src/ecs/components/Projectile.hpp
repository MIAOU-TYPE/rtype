/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Projectile
*/

#pragma once
#include "Entity.hpp"

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Projectile
     * @brief Component representing a projectile entity.
     */
    struct Projectile {
        size_t shooter; ///> ID of the entity that fired the projectile
    };
} // namespace Ecs