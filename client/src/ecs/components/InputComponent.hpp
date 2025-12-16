/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputComponent
*/

#pragma once
#include <cstdint>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @enum ClientCommand
     * @brief Enumeration of client command types.
     */
    enum class ClientCommand : uint8_t { Input = 1 };

    /**
     * @struct InputComponent
     * @brief Stores the current input state for a controllable entity.
     */
    struct InputComponent {
        uint8_t type;  ///> Type of the command (ClientCommand::Input)
        uint8_t up;    ///> State of the 'up' input
        uint8_t down;  ///> State of the 'down' input
        uint8_t left;  ///> State of the 'left' input
        uint8_t right; ///> State of the 'right' input
    };
} // namespace Ecs
