/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEvent
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include "IEvent.hpp"

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class SFMLEvent
     * @brief SFML implementation of the IEvent interface.
     *
     * This class wraps the SFML Event to provide a graphics library-agnostic interface.
     */
    class SFMLEvent : public IEvent {
      public:
        /**
         * @brief Constructor for SFMLEvent.
         * @param sfEvent The SFML event to wrap.
         */
        explicit SFMLEvent(const sf::Event &sfEvent);

        /**
         * @brief Gets the type of the event.
         * @return The type of the event.
         */
        EventType getType() const override;

        /**
         * @brief Checks if the event is of a specific type.
         * @param type The type to check for.
         * @return True if the event is of the specified type, false otherwise.
         */
        bool isType(EventType type) const override;

        /**
         * @brief Checks if this event is a mouse button press event.
         * @return True if the event is a mouse button press, false otherwise.
         */
        bool isMouseButtonPressed() const override;

        /**
         * @brief Checks if this event is a mouse button release event.
         * @return True if the event is a mouse button release, false otherwise.
         */
        bool isMouseButtonReleased() const override;

        /**
         * @brief Gets mouse position from a mouse moved event.
         * @param x Reference to store the X coordinate.
         * @param y Reference to store the Y coordinate.
         * @return True if the event contains mouse position data, false otherwise.
         */
        bool getMousePosition(float &x, float &y) const override;

        /**
         * @brief Gets the underlying SFML event.
         * @return Reference to the SFML event.
         */
        const sf::Event &getSFMLEvent() const;

      private:
        /**
         * @brief Converts SFML event type to our EventType enum.
         * @return The converted EventType.
         */
        EventType convertEventType() const;

        sf::Event _sfmlEvent; ///> The underlying SFML event
    };

} // namespace Graphics