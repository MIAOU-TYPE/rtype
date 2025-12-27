/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SfmlText
*/

#pragma once

#include <SFML/Graphics/Text.hpp>
#include <memory>

#include "IText.hpp"
#include "SfmlFontManager.hpp"

namespace Graphics
{

    /**
     * @class SfmlText
     * @brief SFML implementation of the IText interface.
     */
    class SfmlText final : public IText {
      public:
        /**
         * @brief Constructor for SfmlText.
         * @param font Font handle to be used for the text.
         * @param fontManager Shared pointer to the SfmlFontManager for font management.
         */
        SfmlText(FontHandle font, const std::shared_ptr<SfmlFontManager> &fontManager);

        /**
         * @brief Sets the string to be displayed.
         * @param text The text string.
         */
        void setString(const std::string &text) override;

        /**
         * @brief Sets the font for the text.
         * @param font Font handle to be set.
         */
        void setFont(FontHandle font) override;

        /**
         * @brief Sets the character size of the text.
         * @param size The character size in pixels.
         */
        void setCharacterSize(unsigned int size) override;

        /**
         * @brief Sets the color of the text.
         * @param color The color to be set.
         */
        void setColor(const Color &color) override;

        /**
         * @brief Sets the position of the text.
         * @param x The x-coordinate.
         * @param y The y-coordinate.
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
         * @return Constant reference to the sf::Text object.
         */
        const sf::Text &get() const;

      private:
        std::shared_ptr<SfmlFontManager> _fontManager; ///> Shared pointer to the font manager
        std::unique_ptr<sf::Text> _text;               ///> SFML text object

        std::pair<float, float> _position = {0, 0}; ///> Text position
        Color _color;                               ///> Text color
        unsigned int _characterSize = 30;           ///> Character size
        std::string _content = "";                  ///> Text content
    };
} // namespace Graphics
