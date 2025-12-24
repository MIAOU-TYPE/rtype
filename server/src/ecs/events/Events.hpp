#pragma once

#include <functional>
#include <typeindex>
#include <vector>
#include "eventsPart.hpp"
#include <unordered_map>

namespace Ecs
{
    /**
     * @class Events
     * @brief Simple event system for subscribing and emitting events.
     * Allows components and systems to communicate via events.
     */
    class Events {
      public:
        /**
         * @brief Subscribe to an event type with a callback.
         * @tparam Event Type of the event to subscribe to.
         * @param callback Function to call when the event is emitted.
         */
        template <typename Event>
        void subscribe(std::function<void(const Event &)> callback);

        /**
         * @brief Emit an event to all subscribed listeners.
         * @tparam Event Type of the event to emit.
         * @param event The event instance to emit.
         */
        template <typename Event>
        void emit(const Event &event) const;

      private:
        using fn = std::function<void(const void *)>; ///> Generic function type for event callbacks

        std::unordered_map<std::type_index, std::vector<fn>>
            _subscribers; ///> Map of event type to subscriber callbacks
    };
} // namespace Ecs

#include "Events.tpp"