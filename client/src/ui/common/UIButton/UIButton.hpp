/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UIButton
*/

#pragma once
#include <utility>
#include "IRenderer.hpp"
#include "RenderCommand.hpp"
#include "SfmlText.hpp"

namespace UI
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

    /** @class UIButtonError
     * @brief Exception class for UIButton errors.
     */
    class UIButtonError : public std::exception {
      public:
        /** @brief Constructor for UIButtonError.
         * @param message Error message.
         */
        explicit UIButtonError(std::string message) : _message(std::move(message))
        {
        }

        /** @brief Get the error message.
         * @return Error message as a C-string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
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
         * @brief Handle mouse release event.
         * @param x X coordinate of the mouse.
         * @param y Y coordinate of the mouse.
         * @return True if the button was released, false otherwise.
         */
        [[nodiscard]] bool onMouseReleased(float x, float y);

        /**
         * @brief Handle click release event with a callback.
         * @tparam Fn Type of the callback function.
         * @param x X coordinate of the mouse.
         * @param y Y coordinate of the mouse.
         * @param fn Callback function to execute on click release.
         * @return True if the button was clicked, false otherwise.
         */
        template <typename Fn>
        [[nodiscard]] bool onClickReleased(float x, float y, Fn &&fn);

        /**
         * @brief Handle click press event with a callback.
         * @tparam Fn Type of the callback function.
         * @param x X coordinate of the mouse.
         * @param y Y coordinate of the mouse.
         * @param fn Callback function to execute on click press.
         * @return True if the button was clicked, false otherwise.
         */
        template <typename Fn>
        [[nodiscard]] bool onClickPressed(float x, float y, Fn &&fn);

        /**
         * @brief Center the button label.
         * @param centerX X coordinate to center around.
         * @param y Y coordinate of the label.
         * @param label Text label to center.
         * @param labelCenterX Center X coordinate of the label.
         */
        void centerButtonLabel(float centerX, float y, Graphics::IText &label, float labelCenterX) noexcept;

        /**
         * @brief Render the button.
         */
        void render() const;

        /**
         * @brief Get the bounds of the button.
         * @return FloatRect representing the bounds.
         */
        [[nodiscard]] Engine::FloatRect bounds() const noexcept;

        /**
         * @brief Set the label of the button.
         * @param text New text label.
         */
        void setLabel(const std::string &text);

        /**
         * @brief Set the scale of the button.
         * @param scaleX Scale factor for X axis.
         * @param scaleY Scale factor for Y axis.
         */
        void setScale(float scaleX, float scaleY);

        /**
         * @brief Set the design scale of the button.
         * @param scaleX Design scale factor for X axis.
         * @param scaleY Design scale factor for Y axis.
         */
        void setUIScale(float scaleX, float scaleY);

        /**
         * @brief Reset the clicked state of the button.
         */
        void reset();

      private:
        /**
         * @brief Apply visual changes based on the button state.
         */
        void applyStateVisual();

        Graphics::TextureSize _baseSize{0, 0}; ///> Base size of the button
        float _boxX = 0.f;                     ///> X position of the button
        float _boxY = 0.f;                     ///> Y position of the button

        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer for drawing
        std::shared_ptr<Graphics::IText> _text;         ///> Text label of the button

        Engine::RenderCommand _cmd;                 ///> Render command for the button
        ButtonState _state = ButtonState::Released; ///> Current state of the button

        Graphics::TextureHandle _released = Graphics::InvalidTexture; ///> Texture for released state
        Graphics::TextureHandle _hover = Graphics::InvalidTexture;    ///> Texture for hover state
        Graphics::TextureHandle _pressed = Graphics::InvalidTexture;  ///> Texture for pressed state

        float _designScaleX = 1.f; ///> Design scale factor for X axis
        float _designScaleY = 1.f; ///> Design scale factor for Y axis
        float _uiScaleX = 1.f;     ///> UI scale factor for X axis
        float _uiScaleY = 1.f;     ///> UI scale factor for Y axis
    };
} // namespace UI

#include "UIButton.tpp"
