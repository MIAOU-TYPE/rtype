/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** InputState
*/

#pragma once
#include "Key.hpp"

namespace Engine
{
    /**
     * Represents the state of input for a single frame.
     */
    struct InputFrame {
        Key key = Key::Unknown;     ///> The key that was pressed during the frame.
        bool keyPressed = false;    ///> Indicates if the key was pressed during the frame.
        bool keyReleased = false;   ///> Indicates if the key was released during the frame.
        bool mousePressed = false;  ///> Indicates if the mouse button was pressed during the frame.
        bool mouseReleased = false; ///> Indicates if the mouse button was released during the frame.
        float mouseX = 0.f;         ///> The X position of the mouse.
        float mouseY = 0.f;         ///> The Y position of the mouse.
    };

    /**
     * Manages and updates the input state.
     */
    class InputState {
      public:
        /**
         * @brief Sets the mouse position.
         * @param x The X position of the mouse.
         * @param y The Y position of the mouse.
         */
        void setMouse(float x, float y);

        /**
         * @brief Sets the key that was pressed.
         * @param key The key that was pressed.
         */
        void setKey(Key key);

        /**
         * @brief Marks the mouse as released.
         */
        void setMouseReleased();

        /**
         * @brief Marks the mouse as pressed.
         */
        void setMousePressed();

        /**
         * @brief Marks the key as pressed.
         * @param key The key that was pressed.
         */
        void setKeyPressed(Key key);

        /**
         * @brief Marks the key as released.
         * @param key The key that was released.
         */
        void setKeyReleased(Key key);

        /**
         * @brief Consumes the current input frame and resets the state for the next frame.
         * @return The current InputFrame.
         */
        InputFrame consumeFrame();

      private:
        InputFrame _frame; ///> The current input frame state.
    };
} // namespace Engine
