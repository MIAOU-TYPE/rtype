/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AIBrain
*/

#pragma once



/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @enum AIState
     * @brief Represents different states of AI behavior.
     */
    enum class AIState {
        Idle,   ///> AI is immobile or inactive
        Patrol, ///> AI is moving along a predefined path
        Attack, ///> AI is engaging a target
        Flee,   ///> AI is retreating from danger
        Dead    ///> AI is defeated
    };

    /**
     * @struct AIBrain
     * @brief Stores AI state and internal timer.
     */
    struct AIBrain {
        AIState state = AIState::Idle; ///> Current state of the AI
        float timer = 0.f;             ///> Internal timer for state transitions
    };
} // namespace Ecs