/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "ITextureManager.hpp"
#include "RenderCommand.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @brief Class representing the main menu of the game.
     */
    class Menu {
      public:
        /**
         * @brief Construct a new Menu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the menu.
         */
        explicit Menu(const std::shared_ptr<Graphics::IRenderer> &renderer);

        /**
         * @brief Update the menu state.
         * @param mouseX The x-coordinate of the mouse.
         * @param mouseY The y-coordinate of the mouse.
         */
        void update(float mouseX, float mouseY);

        /**
         * @brief Render the menu.
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
         * @brief Check if the user wants to start the game.
         *
         * @return true if the user wants to start the game, false otherwise.
         */
        [[nodiscard]] bool wantsToStart() const noexcept;

        /**
         * @brief Check if the user wants to quit the game.
         *
         * @return true if the user wants to quit the game, false otherwise.
         */
        [[nodiscard]] bool wantsToQuit() const noexcept;

        /**
         * @brief Check if the user wants to access the settings.
         *
         * @return true if the user wants to access the settings, false otherwise.
         */
        [[nodiscard]] bool wantsSettings() const noexcept;

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer.

        Graphics::TextureHandle _backgroundTexture; ///> Handle to the background texture.
        Graphics::TextureHandle _logoTexture;       ///> Handle to the logo texture.

        RenderCommand _backgroundCmd; ///> Render command for the background.
        RenderCommand _logoCmd;       ///> Render command for the logo.

        std::unique_ptr<UIButton> _play;     ///> Button to start the game.
        std::unique_ptr<UIButton> _settings; ///> Button to access settings.
        std::unique_ptr<UIButton> _quit;     ///> Button to quit the game.

        bool _startRequested = false;    ///> Flag indicating if the user requested to start the game.
        bool _quitRequested = false;     ///> Flag indicating if the user requested to quit.
        bool _settingsRequested = false; ///> Flag indicating if the user requested to access settings.
    };
} // namespace Engine
