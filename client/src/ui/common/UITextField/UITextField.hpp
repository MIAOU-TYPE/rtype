/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UITextField
*/

#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "Key.hpp"

namespace UI
{
    /**
     * @brief A simple rectangle structure for hit detection.
     */
    struct Rect {
        float x; ///> The x-coordinate of the rectangle's top-left corner.
        float y; ///> The y-coordinate of the rectangle's top-left corner.
        float w; ///> The width of the rectangle.
        float h; ///> The height of the rectangle.

        /**
         * @brief Check if a point is inside the rectangle.
         * @param px The x-coordinate of the point.
         * @param py The y-coordinate of the point.
         * @return True if the point is inside the rectangle, false otherwise.
         */
        [[nodiscard]] bool contains(const float px, const float py) const noexcept
        {
            return px >= x && px <= x + w && py >= y && py <= y + h;
        }
    };

    /**
     * @brief A simple UI text field component for user input.
     */
    class UITextField {
      public:
        /**
         * @brief Construct a new UITextField object.
         * @param renderer The graphics renderer to use for rendering text.
         * @param label The label text for the text field.
         * @param password Whether the text field is for password input (hides input).
         */
        UITextField(std::shared_ptr<Graphics::IRenderer> renderer, std::string label, bool password = false);

        /**
         * @brief Destroy the UITextField object.
         */
        void setPosition(float x, float y);

        /**
         * @brief Set the width of the text field.
         * @param w The width to set.
         */
        void setWidth(float w);

        /**
         * @brief Clear the text field's value.
         */
        void clear();

        /**
         * @brief Set whether the text field is focused.
         * @param v True to focus the text field, false to unfocus.
         */
        void setFocused(bool v);

        /**
         * @brief Check if the text field is focused.
         * @return True if focused, false otherwise.
         */
        [[nodiscard]] bool isFocused() const noexcept;

        /**
         * @brief Handle mouse press events.
         * @param mx The x-coordinate of the mouse press.
         * @param my The y-coordinate of the mouse press.
         */
        void onMousePressed(float mx, float my);

        /**
         * @brief Handle key press events.
         * @param key The key that was pressed.
         */
        void onKeyPressed(Engine::Key key);

        /**
         * @brief Layout the text field (calculate sizes and positions).
         */
        void layout();

        /**
         * @brief Render the text field.
         */
        void render() const;

        /**
         * @brief Get the current value of the text field.
         * @return The text field's value.
         */
        [[nodiscard]] const std::string &value() const noexcept;

        /**
         * @brief Set the value of the text field.
         * @param v The value to set.
         */
        void setValue(std::string v);

        /**
         * @brief Get the bounding rectangle of the text field.
         * @return The bounding rectangle.
         */
        [[nodiscard]] Rect bounds() const noexcept;

      private:
        /**
         * @brief Key to character conversion helper.
         * @param key The key to convert.
         * @param out The output character.
         * @return True if conversion was successful, false otherwise.
         */
        static bool keyToChar(Engine::Key key, char &out) noexcept;

        std::shared_ptr<Graphics::IRenderer> _renderer; ///> The graphics renderer.
        std::unique_ptr<Graphics::IText> _labelText;    ///> The label text object.
        std::unique_ptr<Graphics::IText> _valueText;    ///> The value text object.

        std::string _label; ///> The label string.
        std::string _value; ///> The current value string.

        float _x = 0.f;   ///> The x-coordinate of the text field.
        float _y = 0.f;   ///> The y-coordinate of the text field.
        float _w = 420.f; ///> The width of the text field.
        float _h = 40.f;  ///> The height of the text field.

        bool _focused = false;    ///> Whether the text field is focused.
        bool _password = false;   ///> Whether the text field is for password input.
        size_t _maxLen = 32; ///> The maximum length of the input value.
    };
} // namespace UI
