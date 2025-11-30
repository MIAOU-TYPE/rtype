/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** InputEvents
*/

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @namespace Events
 * @brief Contains all event-related classes and interfaces.
 */
namespace Events
{
    /**
     * @enum InputAction
     * @brief Enumeration of game input actions.
     *
     * This enum represents high-level game actions rather than raw keyboard keys,
     * making the system more maintainable and flexible.
     */
    enum class InputAction { MoveUp, MoveDown, MoveLeft, MoveRight, Shoot, Pause, Quit, Confirm, Cancel };

    /**
     * @enum InputState
     * @brief State of an input action.
     */
    enum class InputState { Pressed, Released, Held };

    /**
     * @struct InputEvent
     * @brief Represents an input event with action and state.
     */
    struct InputEvent {
        InputAction action;
        InputState state;
        float deltaTime = 0.0f; ///< Time since last frame for held actions
    };

    /**
     * @class InputEventHandler
     * @brief Base class for handling input events.
     */
    class InputEventHandler {
      public:
        virtual ~InputEventHandler() = default;

        /**
         * @brief Handles an input event.
         * @param event The input event to handle.
         */
        virtual void onInputEvent(const InputEvent &event) = 0;
    };

    /**
     * @class InputEventManager
     * @brief Manages input event dispatching and handler registration.
     */
    class InputEventManager {
      public:
        /**
         * @brief Registers an event handler for a specific action.
         * @param action The input action to listen for.
         * @param handler Shared pointer to the event handler.
         */
        void registerHandler(InputAction action, std::shared_ptr<InputEventHandler> handler);

        /**
         * @brief Unregisters an event handler for a specific action.
         * @param action The input action to stop listening for.
         * @param handler Shared pointer to the event handler to remove.
         */
        void unregisterHandler(InputAction action, std::shared_ptr<InputEventHandler> handler);

        /**
         * @brief Dispatches an input event to all registered handlers.
         * @param event The input event to dispatch.
         */
        void dispatchEvent(const InputEvent &event);

        /**
         * @brief Updates held actions with delta time.
         * @param deltaTime Time elapsed since last update.
         */
        void updateHeldActions(float deltaTime);

      private:
        std::unordered_map<InputAction, std::vector<std::shared_ptr<InputEventHandler>>> _handlers = {};
        std::unordered_map<InputAction, bool> _heldActions = {};
        std::unordered_map<InputAction, float> _heldTimes = {};
    };

} // namespace Events