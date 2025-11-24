/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** IInputHandler
*/

#pragma once

#include <SFML/Window/Event.hpp>

namespace Input {

/**
 * @enum Key
 * @brief Enumeration of keyboard keys.
 *
 * This enum abstracts keyboard keys to make the input handler independent
 * of the underlying graphics library.
 */
enum class Key {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Escape,
    LControl,
    LShift,
    LAlt,
    LSystem,
    RControl,
    RShift,
    RAlt,
    RSystem,
    Menu,
    LBracket,
    RBracket,
    Semicolon,
    Comma,
    Period,
    Quote,
    Slash,
    Backslash,
    Tilde,
    Equal,
    Hyphen,
    Space,
    Enter,
    Backspace,
    Tab,
    PageUp,
    PageDown,
    End,
    Home,
    Insert,
    Delete,
    Add,
    Subtract,
    Multiply,
    Divide,
    Left,
    Right,
    Up,
    Down,
    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    Pause
};

/**
 * @class IInputHandler
 * @brief Interface for handling user input.
 *
 * This interface abstracts input handling, allowing different implementations
 * (e.g., SFML, SDL) to be used interchangeably.
 */
class IInputHandler {
  public:
    /**
     * @brief Virtual destructor for IInputHandler.
     */
    virtual ~IInputHandler() = default;

    /**
     * @brief Handles an input event.
     * @param event The input event to handle.
     */
    virtual void handleEvent(const sf::Event &event) = 0;

    /**
     * @brief Checks if a key is currently pressed.
     * @param key The key to check.
     * @return True if the key is pressed, false otherwise.
     */
    virtual bool isKeyPressed(Key key) const = 0;
};

} // namespace Input
