/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Menu
*/

#pragma once

#include <memory>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "RenderCommand.hpp"
#include "ITextureManager.hpp"

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
         * @param dt Delta time since the last update.
         */
        void update(float dt);

        /**
         * @brief Render the menu.
         */
        void render() const;

        /**
         * @brief Handle the event when the Enter key is pressed.
         */
        void onEnterPressed();

        /**
         * @brief Handle the event when the Escape key is pressed.
         */
        void onEscapePressed();

        [[nodiscard]] bool wantsToStart() const noexcept;
        [[nodiscard]] bool wantsToQuit() const noexcept;

    private:
        std::shared_ptr<Graphics::IRenderer> _renderer;

        Graphics::TextureHandle _backgroundTexture = Graphics::InvalidTexture;
        Graphics::TextureHandle _logoTexture = Graphics::InvalidTexture;

        RenderCommand _backgroundCmd;
        RenderCommand _logoCmd;

        std::unique_ptr<Graphics::IText> _start;
        std::unique_ptr<Graphics::IText> _quit;

        bool _startRequested = false;
        bool _quitRequested = false;
    };
}
