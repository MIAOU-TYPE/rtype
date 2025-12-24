/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EventsRegistry
*/

#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>
#include <vector>
#include "Events.hpp"
#include <unordered_map>

namespace Ecs
{
    /**
     * @brief Registry for event handling in the ECS framework.
     * Allows subscribing to events and emitting them.
     * Events are processed in a queued manner to ensure order of handling.
     */
    class EventsRegistry {
      public:
        /**
         * @brief Subscribe to a specific event type with a callback function.
         * @tparam Event The type of event to subscribe to.
         * @param callback The function to call when the event is emitted.
         */
        template <typename Event>
        void subscribe(std::function<void(const Event &)> callback);

        /**
         * @brief Emit an event, queuing it for processing
         * @tparam Event The type of event to emit.
         * @param event The event instance to emit.
         */
        template <typename Event>
        void emit(const Event &event);

        /**
         * @brief Process all queued events, invoking their respective handlers.
         */
        void process();

      private:
        using Handler = std::function<void(const void *)>; ///> Type-erased event handler

        std::unordered_map<std::type_index, std::vector<Handler>> _subscribers; ///> Map of event type to their handlers

        /**
         * @brief Struct representing a queued event with its type and data.
         */
        struct QueuedEvent {
            std::type_index type;             ///> Type of the event
            std::shared_ptr<const void> data; ///> Shared pointer to the event data
        };

        std::queue<QueuedEvent> _queue; ///> Queue of events to be processed
    };
} // namespace Ecs

#include "EventsRegistry.tpp"
