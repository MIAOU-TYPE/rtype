/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "RenderCommand.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @brief Class representing the settings menu of the game.
     */
    class SettingsMenu {
      public:
        /**
         * @brief Construct a new Settings Menu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the settings menu.
         */
        explicit SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer);

        /**
         * @brief Update the settings menu state.
         * @param dt Delta time since the last update.
         * @param mouseX Current mouse x-coordinate.
         * @param mouseY Current mouse y-coordinate.
         */
        void update(float dt, float mouseX, float mouseY);

        /**
         * @brief Render the settings menu.
         */
        void render() const;

        /**
         * @brief Handle mouse press events.
         *
         * @param x The x-coordinate of the mouse press.
         * @param y The y-coordinate of the mouse press.
         * @return true if the event was handled, false otherwise.
         */
        bool onMousePressed(float x, float y) const;

        /**
         * @brief Handle mouse release events.
         *
         * @param x The x-coordinate of the mouse release.
         * @param y The y-coordinate of the mouse release.
         * @return true if the event was handled, false otherwise.
         */
        bool onMouseReleased(float x, float y);

        /**
         * @brief Check if the user wants to go back from the settings menu.
         *
         * @return true if the user wants to go back, false otherwise.
         */
        bool wantsBack() const noexcept;

        /**
         * @brief Check if the screen resolution has been changed.
         *
         * @return true if the resolution has been changed, false otherwise.
         */
        bool resolutionChanged() noexcept;

        /**
         * @brief Get the current screen resolution.
         *
         * @return The current screen resolution.
         */
        Graphics::Extent2u currentResolution() const noexcept;

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer used for rendering the settings menu

        Graphics::TextureHandle _backgroundTexture; ///> Texture handle for the background image
        RenderCommand _backgroundCmd;               ///> Render command for the background image

        std::unique_ptr<UIButton> _audio;          ///> Audio settings button
        std::unique_ptr<UIButton> _left;           ///> Decrease resolution button
        std::unique_ptr<UIButton> _right;          ///> Increase resolution button
        std::unique_ptr<UIButton> _back;           ///> Back button
        std::unique_ptr<UIButton> _resolution;     ///> Current resolution display button
        std::unique_ptr<UIButton> _resolutionNext; ///> Next resolution button

        bool _backRequested = false; ///> Flag indicating if the user wants to go back

        /**
         * @brief List of available screen resolutions.
         */
        std::vector<Graphics::Extent2u> _resolutions{{900, 600}, {1280, 720}, {1920, 1080}};
        std::size_t _currentResolution = 1; ///> Index of the current screen resolution
        bool _resolutionChanged = false;    ///> Flag indicating if the resolution has been changed
    };
} // namespace Engine
