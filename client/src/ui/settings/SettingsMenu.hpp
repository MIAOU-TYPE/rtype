/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#pragma once

#include <memory>
#include <algorithm>
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

    private:
        std::shared_ptr<Graphics::IRenderer> _renderer;

        Graphics::TextureHandle _backgroundTexture;
        RenderCommand _backgroundCmd;

        std::unique_ptr<UIButton> _audio;
        std::unique_ptr<UIButton> _left;
        std::unique_ptr<UIButton> _right;
        std::unique_ptr<UIButton> _back;

        bool _backRequested = false;
    };
}
