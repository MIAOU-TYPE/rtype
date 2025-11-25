/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLSpriteManagement
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "ITextureManager.hpp"

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class SFMLSprite
     * @brief SFML implementation of the ISprite interface.
     */
    class SFMLSprite : public ISprite {
      public:
        /**
         * @brief Constructor for SFMLSprite.
         * @param sprite The SFML sprite to wrap.
         */
        explicit SFMLSprite(const sf::Sprite &sprite);

        /**
         * @brief Sets the position of the sprite.
         * @param x The X coordinate.
         * @param y The Y coordinate.
         */
        void setPosition(float x, float y) override;

        /**
         * @brief Gets the width of the sprite.
         * @return The width of the sprite.
         */
        float getWidth() const override;

        /**
         * @brief Gets the height of the sprite.
         * @return The height of the sprite.
         */
        float getHeight() const override;

        /**
         * @brief Sets the scale of the sprite.
         * @param scaleX The X scale factor.
         * @param scaleY The Y scale factor.
         */
        void setScale(float scaleX, float scaleY) override;

        /**
         * @brief Gets the underlying SFML sprite.
         * @return Reference to the SFML sprite.
         */
        const sf::Sprite &getSFMLSprite() const;

      private:
        sf::Sprite _sprite; ///> The underlying SFML sprite
    };

} // namespace Graphics