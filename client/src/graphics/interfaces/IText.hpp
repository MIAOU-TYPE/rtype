/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** IText
*/

#pragma once

#include <string>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class IText
     * @brief Interface for text rendering.
     *
     * This interface abstracts text rendering functionality to make it independent
     * of the underlying graphics library.
     */
    class IText {
      public:
        /**
         * @brief Virtual destructor for IText.
         */
        virtual ~IText() = default;

        /**
         * @brief Sets the text string.
         * @param text The text to display.
         */
        virtual void setString(const std::string &text) = 0;

        /**
         * @brief Sets the character size.
         * @param size The size in pixels.
         */
        virtual void setCharacterSize(unsigned int size) = 0;

        /**
         * @brief Sets the text color.
         * @param r Red component (0-255).
         * @param g Green component (0-255).
         * @param b Blue component (0-255).
         * @param a Alpha component (0-255).
         */
        virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) = 0;

        /**
         * @brief Sets the position of the text.
         * @param x The X coordinate.
         * @param y The Y coordinate.
         */
        virtual void setPosition(float x, float y) = 0;

        /**
         * @brief Gets the width of the text.
         * @return The width of the text.
         */
        virtual float getWidth() const = 0;

        /**
         * @brief Gets the height of the text.
         * @return The height of the text.
         */
        virtual float getHeight() const = 0;
    };

} // namespace Graphics
