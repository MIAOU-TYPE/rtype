/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Renderable
*/

#pragma once

#include <SFML/Graphics/Color.hpp>

namespace Ecs
{
    /**
     * @struct Renderable
     * @brief Composant for defining rendering properties of an entity.
     * This component specifies how an entity should be rendered,
     * including visibility, opacity, rotation, scaling, and color tint.
     */
    struct Renderable {
        bool visible = true;               ///> Renderable or not
        float opacity = 1.0f;              ///> 0.0 (invisible) to 1.0 (opaque)
        float rotation = 0.f;              ///> In degrees (0-360)
        float scaleX = 1.f;                ///> Horizontal scale
        float scaleY = 1.f;                ///> Vertical scale
        sf::Color tint = sf::Color::White; ///> Color tint
    };
} // namespace Ecs
