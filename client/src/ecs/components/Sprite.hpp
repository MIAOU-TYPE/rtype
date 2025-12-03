/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Sprite
*/

#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <string>

namespace Ecs {
    /**
     * @struct Sprite
     * @brief Stocks SFML sprite information for an entity.
     * This component holds a shared pointer to an SFML sprite,
     * along with texture reference and rendering layer.
     */
    struct Sprite {
        std::shared_ptr<sf::Sprite> sprite = nullptr;  // The actual SFML sprite object
        std::string textureKey = "";                   // Reference to the texture
        sf::IntRect textureRect = {};                  // Texture area (spritesheet)
        float layer = 0.f;                             // Z-order (0=background, 100=foreground)
    };
}