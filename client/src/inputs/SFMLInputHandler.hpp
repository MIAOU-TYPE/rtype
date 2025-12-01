/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLInputHandler
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include "InputEvents.hpp"

/**
 * @namespace Input
 * @brief Contains all input-related classes and interfaces.
 */
namespace Input
{
    /**
     * @class SFMLInputHandlerError
     * @brief Exception class for SFML input handler-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class SFMLInputHandlerError : public std::exception {
      public:
        /**
         * @brief Constructor for SFMLInputHandlerError.
         * @param message The error message to be associated with the exception.
         */
        explicit SFMLInputHandlerError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @class SFMLInputHandler
     * @brief Modern SFML input handler with pure event-driven architecture.
     *
     * This class converts SFML input events to game action events and dispatches
     * them through our modern event system. Clean and simple!
     */
    class SFMLInputHandler {
      public:
        /**
         * @brief Constructor.
         * @param eventManager Shared pointer to the input event manager.
         */
        explicit SFMLInputHandler(std::shared_ptr<Events::InputEventManager> eventManager);

        /**
         * @brief Handles SFML input events.
         * @param event The SFML event to handle.
         */
        void handleEvent(const sf::Event &event);

        /**
         * @brief Updates the input handler (processes held keys).
         * @param deltaTime Time elapsed since last update.
         */
        void update(float deltaTime);

        /**
         * @brief Maps a physical key to a game action.
         * @param key The SFML keyboard key.
         * @param action The game action to map to.
         */
        void mapKey(sf::Keyboard::Key key, Events::InputAction action);

        /**
         * @brief Unmaps a physical key.
         * @param key The SFML keyboard key to unmap.
         */
        void unmapKey(sf::Keyboard::Key key);

        /**
         * @brief Checks if a key is mapped to any action.
         * @param key The SFML keyboard key.
         * @return True if the key is mapped, false otherwise.
         */
        bool isMapped(sf::Keyboard::Key key) const;

        /**
         * @brief Checks if a specific SFML key is currently pressed.
         * @param key The SFML key to check.
         * @return True if the key is pressed, false otherwise.
         */
        bool isKeyPressed(sf::Keyboard::Key key) const;

      public:
        /**
         * @brief Loads default key mappings.
         * Made public so users can reload defaults if needed.
         */
        void loadDefaultMappings();

      private:
        /**
         * @brief Handles keyboard events (internal processing).
         * @param event The SFML keyboard event.
         */
        void handleKeyboardEvent(const sf::Event &event);

        std::shared_ptr<Events::InputEventManager> _eventManager = nullptr;
        std::unordered_map<sf::Keyboard::Key, Events::InputAction> _keyMappings = {};
    };

} // namespace Input