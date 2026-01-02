/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** ScrollingBackground
*/

#pragma once

namespace Ecs
{

    struct ScrollingBackground {
        float scrollSpeed = 0.0f;  ///> Speed at which the background scrolls
        float spriteWidth = 0.0f;  ///> Width of the background sprite
        float spriteHeight = 0.0f; ///> Height of the background sprite
    };
} // namespace Ecs
