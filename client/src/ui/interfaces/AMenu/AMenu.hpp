/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AMenu
*/

#pragma once
#include <algorithm>
#include <exception>
#include <utility>
#include "IMenu.hpp"

namespace Engine
{
    /**
     * @brief Exception class for menu-related errors.
     */
    class AMenuError : public std::exception {
      public:
        /**
         * @brief Construct a new AMenuError object with a message.
         *
         * @param msg The error message.
         */
        explicit AMenuError(const std::string &msg) : _message(msg)
        {
        }

        /**
         * @brief Get the error message.
         *
         * @return const char* The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Structure representing a rectangle with floating-point coordinates.
     */
    struct RectF {
        float x = 0.f; ///> X-coordinate
        float y = 0.f; ///> Y-coordinate
        float w = 0.f; ///> Width
        float h = 0.f; ///> Height

        /**
         * @brief Get the center x-coordinate of the rectangle.
         *
         * @return float The center x-coordinate.
         */
        [[nodiscard]] float cx() const noexcept;

        /**
         * @brief Get the center y-coordinate of the rectangle.
         *
         * @return float The center y-coordinate.
         */
        [[nodiscard]] float cy() const noexcept;

        /**
         * @brief Get the right x-coordinate of the rectangle.
         *
         * @return float The right x-coordinate.
         */
        [[nodiscard]] float right() const noexcept;

        /**
         * @brief Get the bottom y-coordinate of the rectangle.
         *
         * @return float The bottom y-coordinate.
         */
        [[nodiscard]] float bottom() const noexcept;
    };

    class AMenu : public IMenu {
      public:
        /**
         * @brief Copy constructor is deleted to prevent copying.
         */
        AMenu(const AMenu &) = delete;
        AMenu &operator=(const AMenu &) = delete;

        /**
         * @brief Construct a new AMenu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the menu.
         */
        explicit AMenu(std::shared_ptr<Graphics::IRenderer> renderer);

        /**
         * @brief Virtual destructor for AMenu.
         */
        ~AMenu() override = default;

      protected:
        /**
         * @brief Structure representing the viewport dimensions and center.
         */
        struct ViewportF {
            float w = 0.f;  ///> Width of the viewport
            float h = 0.f;  ///> Height of the viewport
            float cx = 0.f; ///> Center x-coordinate
            float cy = 0.f; ///> Center y-coordinate
        };

        /**
         * @brief Get the current viewport dimensions and center.
         * @return ViewportF structure containing width, height, and center coordinates.
         */
        [[nodiscard]] ViewportF viewportF() const noexcept;

        /**
         * @brief Load the shared background texture.
         */
        void loadBackground(const std::string &path);

        /**
         * @brief Compute the background RenderCommand so it covers the full viewport.
         */
        void layoutBackground() noexcept;

        /**
         * @brief Render the background.
         */
        void renderBackground() const;

        void loadPanel(const std::string &path, bool center = true);
        void layoutPanel(float maxWFrac = 0.90f, float maxHFrac = 0.90f, float insetXFrac = 0.08f,
            float insetYFrac = 0.10f) noexcept;

        [[nodiscard]] RectF panelRect() const noexcept
        {
            return _panelRect;
        }

        [[nodiscard]] RectF innerRect() const noexcept
        {
            return _innerRect;
        }

        /**
         * @brief Helper to place a button centered at a given x-coordinate.
         * @param b The button to place.
         * @param cx The center x-coordinate.
         * @param y The y-coordinate.
         */
        static void placeCentered(UI::UIButton &b, float cx, float y) noexcept;

        /**
         * @brief Helper to layout a stepper control (label with up/down buttons).
         * @param label The label button.
         * @param down The down button.
         * @param up The up button.
         * @param colX The center x-coordinate for the label.
         * @param y The y-coordinate.
         * @param margin The margin between the label and buttons.
         */
        static void layoutStepper(
            UI::UIButton &label, UI::UIButton &down, UI::UIButton &up, float colX, float y, float margin) noexcept;

        /**
         * @brief Helper to layout a choice control with a "next" button.
         * @param mainBtn The main choice button.
         * @param nextBtn The "next" button.
         * @param colX The center x-coordinate for the main button.
         * @param y The y-coordinate.
         * @param labelMargin The margin between the main button and the next button.
         * @param spacingX The horizontal spacing between the main button and the next button.
         * @param viewW The width of the view.
         * @param rightMargin The right margin to respect.
         */
        static void layoutChoiceWithNext(UI::UIButton &mainBtn, UI::UIButton &nextBtn, float colX, float y,
            float labelMargin, float spacingX, float viewW, float rightMargin) noexcept;

        /**
         * @brief Helper to layout two buttons centered as a row.
         * @param left The left button.
         * @param right The right button.
         * @param cx The center x-coordinate for the row.
         * @param y The y-coordinate.
         * @param gap The gap between the two buttons.
         */
        static void layoutRowCentered(UI::UIButton &left, UI::UIButton &right, float cx, float y, float gap) noexcept;

        /**
         * @brief Helper to layout multiple buttons centered in a column.
         * @param viewW The width of the view.
         * @param yStart The starting y-coordinate.
         * @param yStep The vertical step between buttons.
         * @param buttons An initializer list of button pointers to layout.
         */
        static void layoutColumnCentered(
            float viewW, float yStart, float yStep, std::initializer_list<UI::UIButton *> buttons) noexcept;

        /**
         * @brief Helper to pick an action based on button clicks.
         *
         * @tparam Action The type of action to return.
         * @param mx The mouse x-coordinate.
         * @param my The mouse y-coordinate.
         * @param items An initializer list of button-action pairs.
         * @return The action corresponding to the clicked button, or Action::None if no button was clicked.
         */
        template <typename Action>
        [[nodiscard]] Action pickAction(
            float mx, float my, std::initializer_list<std::pair<UI::UIButton *, Action>> items) const noexcept;

        /**
         * @brief Helper to reset multiple buttons at once.
         * @param b Variadic list of button pointers to reset.
         */
        template <typename... B>
        void resetButtons(B *...b) const noexcept;

        /**
         * @brief Helper to update multiple buttons at once.
         * @param mx The mouse x-coordinate.
         * @param my The mouse y-coordinate.
         * @param b Variadic list of button pointers to update.
         */
        template <typename... B>
        void updateButtons(float mx, float my, B *...b) const noexcept;

        /**
         * @brief Helper to press multiple buttons at once.
         * @param mx The mouse x-coordinate.
         * @param my The mouse y-coordinate.
         * @param b Variadic list of button pointers to press.
         */
        template <typename... B>
        void pressButtons(float mx, float my, B *...b) const noexcept;

        std::shared_ptr<Graphics::IRenderer> _renderer;                        ///> Shared renderer
        Graphics::TextureHandle _backgroundTexture = Graphics::InvalidTexture; ///> Background texture
        RenderCommand _backgroundCmd{};                                        ///> Background render command

        Graphics::TextureHandle _panelTex = Graphics::InvalidTexture; ///> Panel texture
        RenderCommand _panelCmd{};                                    ///> Panel render command
        bool _panelCenter = true;                                     ///> Whether to center the panel

        RectF _panelRect{};             ///> Panel rectangle
        RectF _innerRect{};             ///> Inner rectangle within the panel
        float _panelInsetXFrac = 0.08f; ///> Panel inset fraction in X
        float _panelInsetYFrac = 0.10f; ///> Panel inset fraction in Y
    };
} // namespace Engine

#include "AMenu.tpp"