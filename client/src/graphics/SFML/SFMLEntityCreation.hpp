/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityCreation
*/

#pragma once

#include <memory>
#include <string>
#include "IRenderer.hpp"
#include "ITextureManager.hpp"

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class GraphicalEntity
     * @brief Represents a graphical entity with position and sprite.
     *
     * This class handles the graphical representation of game entities
     * with a position and associated sprite.
     */
    class GraphicalEntity {
      public:
        /**
         * @brief Constructor for GraphicalEntity.
         * @param x The X position of the entity.
         * @param y The Y position of the entity.
         * @param spriteName The name of the sprite (without path).
         * @param textureManager Shared pointer to the texture manager.
         */
        GraphicalEntity(
            float x, float y, const std::string &spriteName, std::shared_ptr<ITextureManager> textureManager);

        /**
         * @brief Destructor for GraphicalEntity.
         */
        ~GraphicalEntity() = default;

        /**
         * @brief Sets the position of the entity.
         * @param x The new X position.
         * @param y The new Y position.
         */
        void setPosition(float x, float y);

        /**
         * @brief Gets the X position of the entity.
         * @return The X position.
         */
        float getX() const
        {
            return _x;
        }

        /**
         * @brief Gets the Y position of the entity.
         * @return The Y position.
         */
        float getY() const
        {
            return _y;
        }

        /**
         * @brief Gets the sprite name.
         * @return The sprite name.
         */
        const std::string &getSpriteName() const
        {
            return _spriteName;
        }

        /**
         * @brief Gets the width of the entity sprite.
         * @return The width in pixels.
         */
        float getWidth() const;

        /**
         * @brief Gets the height of the entity sprite.
         * @return The height in pixels.
         */
        float getHeight() const;

        /**
         * @brief Renders the entity.
         * @param renderer The renderer to use.
         */
        void render(const std::shared_ptr<IRenderer> &renderer);

      private:
        float _x;                                         ///> X position
        float _y;                                         ///> Y position
        std::string _spriteName;                          ///> Name of the sprite
        std::shared_ptr<ITextureManager> _textureManager; ///> Texture manager
        std::unique_ptr<ISprite> _sprite;                 ///> The sprite instance
    };

} // namespace Graphics