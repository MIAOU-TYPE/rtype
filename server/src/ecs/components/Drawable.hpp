/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Drawable
*/

#pragma once

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
        std::string sprite;  //< Sprite identifier or path
        bool drawable = true; //< If false, entity is not rendered
    };
} // namespace Ecs