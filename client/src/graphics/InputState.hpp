/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** InputState
*/

#pragma once
#include "Key.hpp"
#include <unordered_set>

namespace Engine
{
    /**
     * @brief Struct representing the state of user input.
     */
    struct InputState {
        float mouseX = 0.f; ///> Current mouse X position
        float mouseY = 0.f; ///> Current mouse Y position

        std::unordered_set<Key> keysPressed; ///> Set of currently pressed keys

        bool mouseLeftDown = false;     ///> Is the left mouse button currently down
        bool mouseLeftPressed = false;  ///> Was the left mouse button pressed this frame
        bool mouseLeftReleased = false; ///> Was the left mouse button released this frame

        /**
         * @brief Checks if a specific key is currently pressed.
         *
         * @param k The key to check.
         * @return true if the key is pressed, false otherwise.
         */
        bool isPressed(const Key k) const
        {
            return keysPressed.contains(k);
        }

        /**
         * @brief Resets the input state for the next frame.
         */
        void resetFrame()
        {
            keysPressed.clear();
            mouseLeftPressed = false;
            mouseLeftReleased = false;
        }
    };
} // namespace Engine
