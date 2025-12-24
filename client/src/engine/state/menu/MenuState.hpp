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
#include "InputState.hpp"
#include "Menu.hpp"
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
         * @param input Shared pointer to the input state.
         */
        explicit MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<InputState> input);

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
         */
        void update() override;

        /**
         * @brief Render the menu state.
         */
        void render() override;

        /**
         * @brief Handle mouse press events.
         *
         * @param x The x-coordinate of the mouse press.
         * @param y The y-coordinate of the mouse press.
         * @return true if the event was handled, false otherwise.
         */
        bool onMousePressed(float x, float y) override;

        /**
         * @brief Handle mouse release events.
         *
         * @param x The x-coordinate of the mouse release.
         * @param y The y-coordinate of the mouse release.
         * @return true if the event was handled, false otherwise.
         */
        bool onMouseReleased(float x, float y) override;

      private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Shared pointer to the graphics interface.
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer interface.
        std::unique_ptr<EventRegistry> _eventRegistry;  ///> Unique pointer to the event registry.
        std::shared_ptr<InputState> _input;             ///> Shared pointer to the input state.
        std::unique_ptr<Menu> _menu;                    ///> Unique pointer to the menu.
        StateManager *_manager = nullptr;               ///> Pointer to the state manager.
    };
} // namespace Engine
