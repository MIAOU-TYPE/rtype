/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Drawable
*/

#pragma once
#include <string>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Drawable
     * @brief Contains rendering information of an entity.
     */
    struct Drawable {
        unsigned int spriteId; ///> Sprite identifier or path
        bool drawable = true;  ///> If false, entity is not rendered
    };
} // namespace Ecs
