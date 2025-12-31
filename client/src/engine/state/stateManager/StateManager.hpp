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
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame);

        /**
         * @brief Render the current state.
         */
        void render() const;

        /**
         * @brief Queue a new state to be switched to.
         * @param state The new game state to switch to.
         */
        void queueState(std::unique_ptr<IGameState> state);

        /**
         * @brief Queue a request to quit the state manager.
         */
        void queueQuit();

        /**
         * @brief Check if the state manager is running.
         *
         * @return true if running, false otherwise.
         */
        bool isRunning() const;

        /**
         * @brief Request to quit the state manager.
         */
        void requestQuit();

      private:
        std::unique_ptr<IGameState> _current; ///> The current game state.
        std::unique_ptr<IGameState> _pending; ///> The pending game state to switch to.
        bool _quitRequested = false;          ///> Indicates if a quit has been requested.
        bool _running = true;                 ///> Indicates if the state manager is running.
    };
} // namespace Engine
