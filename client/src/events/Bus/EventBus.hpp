/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EventBus
*/

#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <typeindex>
#include <vector>
#include <unordered_map>

namespace Core
{
    /**
     * @class EventBus
     * @brief A simple event bus system for handling events and their associated handlers.
     * This class allows registering event handlers for specific event types, emitting events,
     * and dispatching queued events to their respective handlers.
     */
    class EventBus {
      public:
        /**
         * @brief Type alias for an event handler function.
         * @tparam T The type of event the handler will process.
         */
        template <typename T>
        using Handler = std::function<void(const T &)>;

        /**
         * @brief Registers an event handler for a specific event type.
         * @tparam T The type of event the handler will process.
         * @param handler The event handler function to register.
         */
        template <typename T>
        void on(Handler<T> handler);

        /**
         * @brief Emits an event, queuing it for later dispatch.
         * @tparam T The type of event being emitted.
         * @param event The event instance to emit.
         */
        template <typename T>
        void emit(T event);

        /**
         * @brief Dispatches all queued events to their registered handlers.
         * This method processes the event queue, invoking each handler for the corresponding event type.
         */
        void dispatch();

      private:
        /**
         * @struct QueuedEvent
         * @brief Represents an event queued for dispatch, including its type and invocation function.
         */
        struct QueuedEvent {
            std::type_index type;         ///> The type of the event
            std::function<void()> invoke; ///> Function to invoke the event handlers
        };

        std::unordered_map<std::type_index, std::vector<std::function<void(const void *)>>>
            _handlers; ///> Map of event types to their handlers

        std::queue<QueuedEvent> _queue; ///> Queue of events to be dispatched
        std::mutex _queueMutex;         ///> Mutex for thread-safe access to the event queue
    };
} // namespace Core

#include "EventBus.tpp"