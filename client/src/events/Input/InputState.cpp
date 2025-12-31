/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** InputState
*/

#include "InputState.hpp"

namespace Engine
{
    void InputState::setKey(const Key key)
    {
        _frame.key = key;
    }

    void InputState::setMouse(const float x, const float y)
    {
        _frame.mouseX = x;
        _frame.mouseY = y;
    }

    void InputState::setMousePressed()
    {
        _frame.mousePressed = true;
    }

    void InputState::setMouseReleased()
    {
        _frame.mouseReleased = true;
    }

    void InputState::setKeyPressed(const Key key)
    {
        _frame.key = key;
        _frame.keyPressed = true;
    }

    void InputState::setKeyReleased(const Key key)
    {
        _frame.key = key;
        _frame.keyReleased = true;
    }

    InputFrame InputState::consumeFrame()
    {
        const InputFrame out = _frame;

        _frame.mousePressed = false;
        _frame.mouseReleased = false;
        _frame.keyPressed = false;
        _frame.keyReleased = false;
        _frame.key = Key::Unknown;

        return out;
    }
} // namespace Engine
