/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** StateManager
*/

#pragma once

#include <memory>
#include "IGameState.hpp"

namespace Engine
{
    /**
     * @brief Manages the different states of the game.
     */
    class StateManager {
      public:
        /**
         * @brief Construct a new State Manager object with an initial state.
         *
         * @param state The initial game state.
         */
        void changeState(std::unique_ptr<IGameState> state);

        /**
         * @brief Update the current state.
         *
         * @param dt Delta time since last update.
         */
        void update(float dt) const;

        /**
         * @brief Render the current state.
         */
        void render() const;

        /**
         * @brief Check if the state manager is running.
         *
         * @return true if running, false otherwise.
         */
        bool isRunning() const;

      private:
        std::unique_ptr<IGameState> _current; ///> The current game state.
        bool _running = true;                 ///> Indicates if the state manager is running.
    };
} // namespace Engine
