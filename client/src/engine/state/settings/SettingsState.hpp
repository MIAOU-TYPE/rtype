/*
** EPITECH PROJECT, 2025
** MenuState.hpp
** File description:
** SettingsState
*/

#pragma once

#include "SettingsMenu.hpp"
#include "MenuState.hpp"
#include "InputState.hpp"

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
         * @param graphics Shared pointer to the graphics interface.
         * @param renderer Shared pointer to the renderer interface.
         * @param input Shared pointer to the input state.
         */
        explicit SettingsState(
            std::shared_ptr<Graphics::IGraphics> graphics,
            std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<InputState> input);

        /**
         * @brief Destroy the Settings State object.
         */
        void onEnter(StateManager &manager) override;

        /**
         * @brief Clean up resources when exiting the settings state.
         */
        void onExit() override;

        /**
         * @brief Update the settings state.
         *
         * @param dt Delta time since last update.
         */
        void update(float dt) override;

        /**
         * @brief Render the settings state.
         */
        void render() override;

        /**
         * @brief Handle mouse press events.
         *
         * @param x The x-coordinate of the mouse press.
         * @param y The y-coordinate of the mouse press.
         */
        bool onMousePressed(float x, float y) override;

        /**
         * @brief Handle mouse release events.
         *
         * @param x The x-coordinate of the mouse release.
         * @param y The y-coordinate of the mouse release.
         */
        bool onMouseReleased(float x, float y) override;

    private:
        std::shared_ptr<Graphics::IGraphics> _graphics; ///> Graphics interface
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer interface
        std::unique_ptr<SettingsMenu> _menu;            ///> Settings menu
        std::shared_ptr<InputState> _input;             ///> Input state
        StateManager *_manager = nullptr;               ///> Pointer to the state manager
    };
}
