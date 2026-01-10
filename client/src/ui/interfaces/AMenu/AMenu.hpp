/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AMenu
*/

#pragma once
#include "IMenu.hpp"

namespace Engine
{
    /**
     * @brief Abstract base class for menus in the game.
     * This class provides common functionality for all menus, such as background rendering
     * and button management.
     */
    class AMenu : public IMenu {
      public:
        /**
         * @brief Copy constructor is deleted to prevent copying.
         */
        AMenu(const AMenu &) = delete;

        /**
         * @brief Construct a new AMenu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the menu.
         */
        explicit AMenu(std::shared_ptr<Graphics::IRenderer> renderer);

        /**
         * @brief Assignment operator is deleted to prevent copying.
         */
        AMenu &operator=(const AMenu &) = delete;

        /**
         * @brief Virtual destructor for AMenu.
         */
        ~AMenu() override = default;

      protected:
        /**
         * @brief Load the shared background texture.
         */
        void loadBackground(const std::string &path);

        /**
         * @brief Compute the background RenderCommand so it covers the full viewport.
         */
        void layoutBackground();

        /**
         * @brief Render the background.
         */
        void renderBackground() const;

        /**
         * @brief Helper to reset multiple buttons at once.
         */
        template <typename... B>
        static void resetButtons(B *...b) noexcept;

        /**
         * @brief Helper to update multiple buttons at once.
         */
        template <typename... B>
        static void updateButtons(float mx, float my, B *...b) noexcept;

        /**
         * @brief Helper to press multiple buttons at once.
         */
        template <typename... B>
        static void pressButtons(float mx, float my, B *...b) noexcept;

        std::shared_ptr<Graphics::IRenderer> _renderer;                        ///> Shared renderer
        Graphics::TextureHandle _backgroundTexture = Graphics::InvalidTexture; ///> Background texture
        RenderCommand _backgroundCmd{};                                        ///> Background render command
    };
} // namespace Engine

#include "AMenu.tpp"