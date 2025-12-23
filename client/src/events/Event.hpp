/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Event
*/

#pragma once
#include "Key.hpp"

namespace Engine
{
    /**
     * @brief Base class for all events.
     */
    struct Event {
        /**
         * @brief Virtual destructor for Event.
         */
        virtual ~Event() = default;
    };

    /**
     * @brief Event triggered when the window is closed.
     */
    struct WindowClosed : Event {};

    /**
     * @brief Event triggered when a key is pressed.
     */
    struct KeyPressed : Event {
        /**
         * @brief Constructor for KeyPressed event.
         * @param k The key that was pressed.
         */
        explicit KeyPressed(const Key k) : key(k)
        {
        }

        Key key; ///> The key that was pressed.
    };

    /**
     * @brief Event triggered when a key is released.
     */
    struct KeyReleased : Event {
        /**
         * @brief Constructor for KeyReleased event.
         * @param k The key that was released.
         */
        explicit KeyReleased(const Key k) : key(k)
        {
        }

        Key key; ///> The key that was released.
    };

    /**
     * @brief Event triggered when the mouse is moved.
     */
    struct MouseMoved : Event {
        /**
         * @brief Constructor for MouseMoved event.
         * @param x The x position of the mouse.
         * @param y The y position of the mouse.
         */
        explicit MouseMoved(int x, int y) : posX(x), posY(y)
        {
        }

        int posX; ///> The x position of the mouse.
        int posY; ///> The y position of the mouse.
    };

    /**
     * @brief Event triggered when a mouse button is pressed.
     */
    struct MousePressed : Event {
        /**
         * @brief Constructor for MousePressed event.
         * @param k The mouse button that was pressed.
         * @param x The x position of the mouse when pressed.
         * @param y The y position of the mouse when pressed.
         */
        explicit MousePressed(const Key k, const int x, const int y) : key(k), posX(x), posY(y)
        {
        }

        Key key;      ///> The mouse button that was pressed.
        int posX = 0; ///> The x position of the mouse when pressed.
        int posY = 0; ///> The y position of the mouse when
    };

    /**
     * @brief Event triggered when a mouse button is released.
     */
    struct MouseReleased : Event {
        /**
         * @brief Constructor for MouseReleased event.
         * @param k The mouse button that was released.
         */
        explicit MouseReleased(const Key k) : key(k)
        {
        }

        Key key;      ///> The mouse button that was released.
        int posX = 0; ///> The x position of the mouse when released.
        int posY = 0; ///> The y position of the mouse when released.
    };
} // namespace Engine
