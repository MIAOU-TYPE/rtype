/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** IEvent
*/

#pragma once

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @enum EventType
     * @brief Enumeration of all possible event types.
     */
    enum class EventType {
        Closed,                 ///> Window closed event
        Resized,                ///> Window resized event
        LostFocus,              ///> Window lost focus event
        GainedFocus,            ///> Window gained focus event
        TextEntered,            ///> Text entered event
        KeyPressed,             ///> Key pressed event
        KeyReleased,            ///> Key released event
        MouseWheelMoved,        ///> Mouse wheel moved event
        MouseButtonPressed,     ///> Mouse button pressed event
        MouseButtonReleased,    ///> Mouse button released event
        MouseMoved,             ///> Mouse moved event
        MouseEntered,           ///> Mouse entered window event
        MouseLeft,              ///> Mouse left window event
        JoystickButtonPressed,  ///> Joystick button pressed event
        JoystickButtonReleased, ///> Joystick button released event
        JoystickMoved,          ///> Joystick moved event
        JoystickConnected,      ///> Joystick connected event
        JoystickDisconnected,   ///> Joystick disconnected event
        TouchBegan,             ///> Touch began event
        TouchMoved,             ///> Touch moved event
        TouchEnded,             ///> Touch ended event
        SensorChanged,          ///> Sensor changed event
        Unknown                 ///> Unknown event type
    };

    /**
     * @class IEvent
     * @brief Abstract interface for window events.
     *
     * This interface provides an abstraction for window events,
     * independent of the graphics library used.
     */
    class IEvent {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IEvent() = default;

        /**
         * @brief Gets the type of the event.
         * @return The type of the event.
         */
        virtual EventType getType() const = 0;

        /**
         * @brief Checks if the event is of a specific type.
         * @param type The type to check for.
         * @return True if the event is of the specified type, false otherwise.
         */
        virtual bool isType(EventType type) const = 0;
    };

} // namespace Graphics