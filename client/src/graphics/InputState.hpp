/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** InputState
*/

#pragma once

namespace Engine
{
    /**
     * @brief Struct representing the state of user input.
     */
    struct InputState {
        float mouseX = 0.f; ///> Current mouse X position
        float mouseY = 0.f; ///> Current mouse Y position

        bool mouseLeftDown = false; ///> Is the left mouse button currently down
        bool mouseLeftPressed = false;  ///> Was the left mouse button pressed this frame
        bool mouseLeftReleased = false; ///> Was the left mouse button released this frame

        /**
         * @brief Resets the per-frame input states.
         */
        void resetFrame()
        {
            mouseLeftPressed = false;
            mouseLeftReleased = false;
        }
    };
}
