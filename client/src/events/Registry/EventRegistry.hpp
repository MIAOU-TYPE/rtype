/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EventRegistry
*/

#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "EventBus.hpp"
#include "Key.hpp"
#include <unordered_map>

namespace Core
{
    /**
     * @class EventRegistry
     * @brief Registers event handlers for key press and release events.
     * This class allows the registration of callback functions that are invoked
     * when specific keys are pressed or released.
     */
    class EventRegistry {
      public:
        /**
         * @brief Construct a new Event Registry object.
         * @param bus Shared pointer to the EventBus instance.
         */
        explicit EventRegistry(const std::shared_ptr<EventBus> &bus);

        /**
         * @brief Register a callback function to be called when a specific key is pressed.
         * @param key The key to listen for.
         * @param fn The callback function to invoke on key press.
         */
        void onKeyPressed(Key key, std::function<void()> fn);

        /**
         * @brief Register a callback function to be called when a specific key is released.
         * @param key The key to listen for.
         * @param fn The callback function to invoke on key release.
         */
        void onKeyReleased(Key key, std::function<void()> fn);

      private:
        std::shared_ptr<EventBus> _bus; ///> Shared pointer to the EventBus instance

        std::unordered_map<Key, std::vector<std::function<void()>>> _keyPressed;  ///> Map of key press handlers
        std::unordered_map<Key, std::vector<std::function<void()>>> _keyReleased; ///> Map of key release handlers
    };
} // namespace Core
