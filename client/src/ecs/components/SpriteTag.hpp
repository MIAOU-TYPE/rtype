/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteTag
*/

#pragma once
#include <string>

namespace Ecs
{
    /** @struct SpriteTag
     * @brief Component that holds a tag name for sprite identification.
     */
    struct SpriteTag {
        std::string name; ///> The tag name for the sprite.
    };
} // namespace Ecs
