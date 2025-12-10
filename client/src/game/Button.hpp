/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Button
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include "IRenderer.hpp"
#include "ITextureManager.hpp"

/**
 * @namespace Game
 * @brief Contains all game-related classes and interfaces.
 */
namespace Game
{
    /**
     * @enum ButtonState
     * @brief Enumeration of button visual states.
     */
    enum class ButtonState {
        Normal, ///< Normal state
        Hover,  ///< Mouse hovering over button
        Pressed ///< Button being pressed
    };

    /**
     * @class Button
     * @brief Represents an interactive button in the menu.
     */
    class Button {
      public:
        /**
         * @brief Constructor for Button.
         * @param x The X position of the button.
         * @param y The Y position of the button.
         * @param normalTexture Path to normal state texture.
         * @param hoverTexture Path to hover state texture.
         * @param pressedTexture Path to pressed state texture.
         * @param textureManager Shared pointer to the texture manager.
         * @param labelText Text to display on the button.
         * @param fontPath Path to the font file.
         */
        Button(float x, float y, const std::string &normalTexture, const std::string &hoverTexture,
            const std::string &pressedTexture, std::shared_ptr<Graphics::ITextureManager> textureManager,
            const std::string &labelText = "", const std::string &fontPath = "");

        /**
         * @brief Updates the button state based on mouse position and click.
         * @param mouseX The X position of the mouse.
         * @param mouseY The Y position of the mouse.
         * @param isClicked Whether the mouse button is clicked.
         * @return True if button was clicked this frame, false otherwise.
         */
        bool update(float mouseX, float mouseY, bool isClicked);

        /**
         * @brief Renders the button.
         * @param renderer The renderer to use.
         */
        void render(const std::shared_ptr<Graphics::IRenderer> &renderer);

        /**
         * @brief Sets the callback function to be called when button is clicked.
         * @param callback The callback function.
         */
        void setOnClick(std::function<void()> callback);

        /**
         * @brief Gets the X position of the button.
         * @return The X position.
         */
        float getX() const;

        /**
         * @brief Gets the Y position of the button.
         * @return The Y position.
         */
        float getY() const;

        /**
         * @brief Gets the width of the button.
         * @return The width.
         */
        float getWidth() const;

        /**
         * @brief Gets the height of the button.
         * @return The height.
         */
        float getHeight() const;

      private:
        float _x = 0.0f;                                                      ///< X position
        float _y = 0.0f;                                                      ///< Y position
        float _width = 0.0f;                                                  ///< Width of button
        float _height = 0.0f;                                                 ///< Height of button
        ButtonState _state = ButtonState::Normal;                             ///< Current button state
        ButtonState _previousState = ButtonState::Normal;                     ///< Previous button state
        std::unique_ptr<Graphics::ISprite> _normalSprite = nullptr;           ///< Normal state sprite
        std::unique_ptr<Graphics::ISprite> _hoverSprite = nullptr;            ///< Hover state sprite
        std::unique_ptr<Graphics::ISprite> _pressedSprite = nullptr;          ///< Pressed state sprite
        std::unique_ptr<Graphics::IText> _label = nullptr;                    ///< Text label
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///< Texture manager
        std::function<void()> _onClick = nullptr;                             ///< Click callback
        bool _wasPressed = false; ///< Track if button was pressed last frame

        /**
         * @brief Checks if a point is inside the button bounds.
         * @param x The X coordinate.
         * @param y The Y coordinate.
         * @return True if point is inside, false otherwise.
         */
        bool contains(float x, float y) const;
    };

} // namespace Game
