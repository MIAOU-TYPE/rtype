/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GravityAffected
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct GravityAffected
     * @brief Tag component that marks an entity as affected by gravity fields.
     *
     * Entities without this component will be immune to gravity effects.
     */
    struct GravityAffected {
        bool isAffected = true; ///> If false, entity is immune to gravity
    };
} // namespace Ecs
