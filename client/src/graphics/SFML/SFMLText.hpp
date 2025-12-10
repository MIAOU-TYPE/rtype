/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLText
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include "IText.hpp"

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class SFMLText
     * @brief SFML implementation of the IText interface.
     *
     * This class wraps SFML's Text class to provide text rendering functionality.
     */
    class SFMLText : public IText {
      public:
        /**
         * @brief Constructor for SFMLText.
         * @param font Shared pointer to the SFML font.
         * @param text The initial text string.
         * @param characterSize The character size in pixels.
         */
        SFMLText(std::shared_ptr<sf::Font> font, const std::string &text = "", unsigned int characterSize = 30);

        /**
         * @brief Sets the text string.
         * @param text The text to display.
         */
        void setString(const std::string &text) override;

        /**
         * @brief Sets the character size.
         * @param size The size in pixels.
         */
        void setCharacterSize(unsigned int size) override;

        /**
         * @brief Sets the text color.
         * @param r Red component (0-255).
         * @param g Green component (0-255).
         * @param b Blue component (0-255).
         * @param a Alpha component (0-255).
         */
        void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) override;

        /**
         * @brief Sets the position of the text.
         * @param x The X coordinate.
         * @param y The Y coordinate.
         */
        void setPosition(float x, float y) override;

        /**
         * @brief Gets the width of the text.
         * @return The width of the text.
         */
        float getWidth() const override;

        /**
         * @brief Gets the height of the text.
         * @return The height of the text.
         */
        float getHeight() const override;

        /**
         * @brief Gets the underlying SFML text object.
         * @return Reference to the SFML text object.
         */
        const sf::Text &getSFMLText() const;

      private:
        std::shared_ptr<sf::Font> _font; ///> The font used by the text
        sf::Text _text;                  ///> The SFML text object
    };

} // namespace Graphics
