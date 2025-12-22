/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#pragma once

#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "Menu.hpp"

namespace Engine
{
    /**
     * @brief Represents the menu state of the game.
     */
    class MenuState final : public IGameState {
      public:
        /**
         * @brief Construct a new Menu State object.
         *
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         */
        explicit MenuState(
            std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer);

        /**
         * @brief Destroy the Menu State object.
         */
        void onEnter(StateManager &manager) override;

        /**
         * @brief Clean up resources when exiting the menu state.
         */
        void onExit() override;

        /**
         * @brief Update the menu state.
         *
         * @param dt Delta time since last update.
         */
        void update(float dt) override;

        /**
         * @brief Render the menu state.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Shared pointer to the graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer interface.
        std::unique_ptr<Menu> _menu;                    ///> Unique pointer to the menu.
        StateManager *_manager = nullptr;               ///> Pointer to the state manager.
    };
} // namespace Engine
