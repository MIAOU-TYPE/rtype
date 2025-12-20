/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Key
*/

#pragma once

namespace Core
{
    /**
     * @enum Key
     * @brief Enumeration of keyboard and mouse keys.
     */
    enum class Key {
        Unknown = -1, ///> Unknown key

        A, ///> Key A
        B, ///> Key B
        C, ///> Key C
        D, ///> Key D
        E, ///> Key E
        F, ///> Key F
        G, ///> Key G
        H, ///> Key H
        I, ///> Key I
        J, ///> Key J
        K, ///> Key K
        L, ///> Key L
        M, ///> Key M
        N, ///> Key N
        O, ///> Key O
        P, ///> Key P
        Q, ///> Key Q
        R, ///> Key R
        S, ///> Key S
        T, ///> Key T
        U, ///> Key U
        V, ///> Key V
        W, ///> Key W
        X, ///> Key X
        Y, ///> Key Y
        Z, ///> Key Z

        Num0, ///> Number keys 0
        Num1, ///> Number keys 1
        Num2, ///> Number keys 2
        Num3, ///> Number keys 3
        Num4, ///> Number keys 4
        Num5, ///> Number keys 5
        Num6, ///> Number keys 6
        Num7, ///> Number keys 7
        Num8, ///> Number keys 8
        Num9, ///> Number keys 9

        Escape,    ///> Escape key
        Space,     ///> Space key
        Enter,     ///> Enter key
        Tab,       ///> Tab key
        Backspace, ///> Backspace key

        Left,  ///> Arrow keys left
        Right, ///> Arrow keys right
        Up,    ///> Arrow keys up
        Down,  ///> Arrow keys down

        LShift, ///> Left Shift keys
        RShift, ///> Right Shift keys
        LCtrl,  ///> Left Control keys
        RCtrl,  ///> Right Control keys

        MouseLeft, ///> Left mouse button
        MouseRight ///> Right mouse button
    };
} // namespace Core
