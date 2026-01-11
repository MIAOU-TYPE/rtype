/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Net
*/

#pragma once
#include <cstddef>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Id
     * @brief Component representing a unique network identifier for an entity.
     */
    struct Id {
        size_t id = 0; ///> Unique network identifier
    };
} // namespace Ecs