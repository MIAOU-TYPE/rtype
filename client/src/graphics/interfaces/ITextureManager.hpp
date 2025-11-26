/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** ITextureManager
*/

#pragma once

#include <memory>
#include <string>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{

    /**
     * @class ISprite
     * @brief Interface for sprite objects.
     *
     * This interface abstracts sprite functionality to make it independent
     * of the underlying graphics library.
     */
    class ISprite {
      public:
        /**
         * @brief Virtual destructor for ISprite.
         */
        virtual ~ISprite() = default;

        /**
         * @brief Sets the position of the sprite.
         * @param x The X coordinate.
         * @param y The Y coordinate.
         */
        virtual void setPosition(float x, float y) = 0;

        /**
         * @brief Gets the width of the sprite.
         * @return The width of the sprite.
         */
        virtual float getWidth() const = 0;

        /**
         * @brief Gets the height of the sprite.
         * @return The height of the sprite.
         */
        virtual float getHeight() const = 0;

        /**
         * @brief Sets the scale of the sprite.
         * @param scaleX The X scale factor.
         * @param scaleY The Y scale factor.
         */
        virtual void setScale(float scaleX, float scaleY) = 0;

        /**
         * @brief Sets the texture rectangle (sub-area of the texture to display).
         * @param left The left coordinate of the rectangle.
         * @param top The top coordinate of the rectangle.
         * @param width The width of the rectangle.
         * @param height The height of the rectangle.
         */
        virtual void setTextureRect(float left, float top, float width, float height) = 0;
    };

    /**
     * @class ITextureManager
     * @brief Interface for texture management operations.
     *
     * This interface abstracts texture loading and sprite creation,
     * allowing different implementations to be used interchangeably.
     */
    class ITextureManager {
      public:
        /**
         * @brief Virtual destructor for ITextureManager.
         */
        virtual ~ITextureManager() = default;

        /**
         * @brief Loads a texture from file.
         * @param filePath The path to the texture file.
         * @return True if the texture was loaded successfully, false otherwise.
         */
        virtual bool loadTexture(const std::string &filePath) = 0;

        /**
         * @brief Creates a sprite from a loaded texture.
         * @param texturePath The path to the texture file.
         * @return A unique pointer to the created sprite.
         */
        virtual std::unique_ptr<ISprite> createSprite(const std::string &texturePath) = 0;
    };

} // namespace Graphics