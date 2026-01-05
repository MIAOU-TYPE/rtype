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
    /* @brief Forward declaration of StateManager to avoid circular dependency.
     */
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
         */
        virtual void onEnter() = 0;

        /**
         * @brief Updates the state logic.
         * @param manager Reference to the StateManager managing this state.
         * @param frame The current input frame.
         */
        virtual void update(StateManager &manager, const InputFrame &frame) = 0;

        /**
         * @brief Renders the state.
         */
        virtual void render() = 0;
    };
} // namespace Engine
