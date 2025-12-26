/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IGameState
*/

#pragma once
#include "InputState.hpp"

namespace Engine
{
    class StateManager;

    /**
     * @class IGameState
     * @brief Interface for game states in the state management system.
     *
     * This interface defines the essential methods that any game state must implement,
     * including entering and exiting the state, updating the state logic, and rendering.
     */
    class IGameState {
      public:
        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~IGameState() = default;

        /**
         * @brief Called when entering the state.
         * @param manager Reference to the StateManager managing this state.
         */
        virtual void onEnter(StateManager &manager) = 0;

        /**
         * @brief Called when exiting the state.
         */
        virtual void onExit() = 0;

        /**
         * @brief Updates the state logic.
         */
        virtual void update(const InputFrame &frame) = 0;

        /**
         * @brief Renders the state.
         */
        virtual void render() = 0;
    };
} // namespace Engine
