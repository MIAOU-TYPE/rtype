/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#pragma once

#include "EventRegistry.hpp"
#include "IGameState.hpp"
#include "IGraphics.hpp"
#include "IRenderer.hpp"
#include "Menu.hpp"
#include "RoomState.hpp"
#include "SettingsState.hpp"
#include "StateManager.hpp"

namespace Engine
{
    /**
     * @brief Represents the menu state of the game.
     */
    class MenuState : public IGameState {
      public:
        /**
         * @brief Construct a new Menu State object.
         *
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         * @param roomManager Shared pointer to the room manager.
         */
        explicit MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<RoomManager> roomManager);

        /**
         * @brief Called when entering the state.
         */
        void onEnter() override;

        /**
         * @brief Update the menu state.
         * @param manager Reference to the state manager.
         * @param frame The current input frame.
         */
        void update(StateManager &manager, const InputFrame &frame) override;

        /**
         * @brief Render the menu state.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Shared pointer to the graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer interface.

        std::shared_ptr<RoomManager> _roomManager; ///> Lobby/room manager shared across states.
        std::unique_ptr<Menu> _menu;               ///> Unique pointer to the menu.
    };
} // namespace Engine
