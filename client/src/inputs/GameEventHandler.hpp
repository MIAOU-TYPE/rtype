/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameEventHandler
*/

#pragma once

#include <memory>
#include "InputEvents.hpp"

// Forward declaration
namespace Game
{
    class GameScene;
}

/**
 * @namespace Input
 * @brief Contains all input-related classes and interfaces.
 */
namespace Input
{
    /**
     * @class GameEventHandler
     * @brief Handles game-specific input events.
     *
     * This class processes input events and translates them into game actions,
     * providing a clean separation between input handling and game logic.
     */
    class GameEventHandler : public Events::InputEventHandler {
      public:
        /**
         * @brief Constructor.
         * @param gameScene Shared pointer to the game scene to control.
         */
        explicit GameEventHandler(std::shared_ptr<Game::GameScene> gameScene);

        /**
         * @brief Handles an input event.
         * @param event The input event to handle.
         */
        void onInputEvent(const Events::InputEvent &event) override;

      private:
        /**
         * @brief Handles movement actions.
         * @param action The movement action.
         * @param state The input state.
         * @param deltaTime Time for held actions.
         */
        void handleMovement(Events::InputAction action, Events::InputState state, float deltaTime);

        /**
         * @brief Handles shooting actions.
         * @param state The input state.
         */
        void handleShooting(Events::InputState state);

        /**
         * @brief Handles menu/system actions.
         * @param action The menu action.
         * @param state The input state.
         */
        void handleSystemActions(Events::InputAction action, Events::InputState state);

        std::shared_ptr<Game::GameScene> _gameScene = nullptr; ///> Shared pointer to the game scene
    };

} // namespace Input