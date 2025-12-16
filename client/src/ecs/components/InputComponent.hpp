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
    enum class ClientCommand : uint8_t { Input = 1 };

    /**
     * @struct InputComponent
     * @brief Stores the current input state for a controllable entity.
     */
    struct InputComponent {
        uint8_t type;
        uint8_t up;
        uint8_t down;
        uint8_t left;
        uint8_t right;
    };
} // namespace Ecs
