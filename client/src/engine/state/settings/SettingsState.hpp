/*
** EPITECH PROJECT, 2025
** MenuState.hpp
** File description:
** SettingsState
*/

#pragma once

#include "InputState.hpp"
#include "MenuState.hpp"
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
         *
         * @param manager Reference to the state manager.
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         */
        explicit SettingsState(StateManager &manager, std::shared_ptr<Graphics::IGraphics> graphics,
            std::shared_ptr<Graphics::IRenderer> renderer);
        /**
         * @brief Destroy the Settings State object.
         */
        void onEnter(StateManager &manager) override;

        /**
         * @brief Update the menu state.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame) override;

        /**
         * @brief Render the settings state.
         */
        void render() override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Graphics interface
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer interface
        std::unique_ptr<SettingsMenu> _menu;            ///> Settings menu

        StateManager &_manager; ///> Reference to the state manager
        bool _pendingResize = false;
    };
} // namespace Engine
