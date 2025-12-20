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
    struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    using FontHandle = std::size_t;

    class TextError : public std::exception {
      public:
        explicit TextError(const std::string &message) : _message("\n\t" + message)
        {
        }

        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @class IText
     * @brief Interface for text rendering.
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
         * @brief Sets the font for the text.
         * @param font The font handle to use.
         */
        virtual void setFont(FontHandle font) = 0;

        /**
         * @brief Sets the character size.
         * @param size The size in pixels.
         */
        virtual void setCharacterSize(unsigned int size) = 0;

        /**
         * @brief Sets the text color.
         * @param color The color to set.
         */
        virtual void setColor(const Color &color) = 0;

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
