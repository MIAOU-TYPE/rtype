/*
** EPITECH PROJECT, 2025
** MenuState.hpp
** File description:
** UIButton
*/

#pragma once

#include <memory>
#include <string>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "RenderCommand.hpp"
#include "SfmlText.hpp"

namespace Engine
{
    /** @enum ButtonSize
     * @brief Enum representing the size of a button.
     */
    enum class ButtonSize {
        Small, ///> Small
        Large  ///> Large
    };

    /** @enum ButtonState
     * @brief Enum representing the state of a button.
     */
    enum class ButtonState {
        Released, ///> Released
        Hover,    ///> Hover
        Pressed   ///> Pressed
    };

    /** @class UIButton
     * @brief Class representing a UI button.
     */
    class UIButton {
      public:
        /** @brief Constructor for UIButton.
         * @param renderer Shared pointer to the renderer.
         * @param size Size of the button.
         * @param label Text label of the button.
         */
        UIButton(const std::shared_ptr<Graphics::IRenderer> &renderer, ButtonSize size, const std::string &label);

        /**
         * @brief Set the position of the button.
         * @param x X coordinate.
         * @param y Y coordinate.
         */
        void setPosition(float x, float y);

        /**
         * @brief Update the button state based on mouse position.
         * @param mouseX X coordinate of the mouse.
         * @param mouseY Y coordinate of the mouse.
         */
        void update(float mouseX, float mouseY);

        /**
         * @brief Handle mouse press event.
         * @param x X coordinate of the mouse.
         * @param y Y coordinate of the mouse.
         * @return True if the button was pressed, false otherwise.
         */
        bool onMousePressed(float x, float y);

        /**
         * @brief Render the button.
         */
        void render() const;

        /**
         * @brief Get the bounds of the button.
         * @return FloatRect representing the bounds.
         */
        [[nodiscard]] FloatRect bounds() const noexcept;

        /**
         * @brief Handle mouse release event.
         * @param x X coordinate of the mouse.
         * @param y Y coordinate of the mouse.
         * @return True if the button was released, false otherwise.
         */
        bool onMouseReleased(float x, float y);

        /**
         * @brief Check if the button was clicked.
         * @return True if clicked, false otherwise.
         */
        bool wasClicked() const noexcept;

        /**
         * @brief Set the label of the button.
         * @param text New text label.
         */
        void setLabel(const std::string &text) const;

        /**
         * @brief Set the scale of the button.
         * @param scale Scale factor.
         */
        void setScale(float scale);

        /**
         * @brief Reset the clicked state of the button.
         */
        void reset();

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer for drawing
        std::unique_ptr<Graphics::IText> _text;         ///> Text label of the button

        RenderCommand _cmd;                         ///> Render command for the button
        ButtonState _state = ButtonState::Released; ///> Current state of the button

        Graphics::TextureHandle _released; ///> Texture for released state
        Graphics::TextureHandle _hover;    ///> Texture for hover state
        Graphics::TextureHandle _pressed;  ///> Texture for pressed state

        float _scale = 1.f;    ///> Scale factor for the button
        bool _clicked = false; ///> Flag indicating if the button was clicked
    };
} // namespace Engine
