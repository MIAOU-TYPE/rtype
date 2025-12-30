/*
** EPITECH PROJECT, 2025
** MenuState.hpp
** File description:
** SettingsState
*/

#pragma once

#include "InputState.hpp"
#include "MenuState.hpp"
#include "RoomService.hpp"
#include "SettingsMenu.hpp"

namespace Engine
{
    /**
     * @brief Represents the settings state of the game.
     */
    class SettingsState : public IGameState {
      public:
        /**
         * @brief Construct a new Settings State object.
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         */
        explicit SettingsState(std::shared_ptr<Graphics::IGraphics> graphics,
            std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<RoomService> roomService);
        /**
         * @brief Destroy the Settings State object.
         */
        void onEnter(StateManager &manager) override;

        /**
         * @brief Update the menu state.
         * @param manager Reference to the state manager.
         * @param frame The current input frame.
         */
        void update(StateManager &manager, const InputFrame &frame) override;

        /**
         * @brief Render the settings state.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Graphics interface
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer interface
        std::shared_ptr<RoomService> _roomService;      ///> Shared lobby/room service.
        std::unique_ptr<SettingsMenu> _menu;            ///> Settings menu

        bool _pendingResize = false;
    };
} // namespace Engine
