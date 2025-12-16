/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputData
*/

#pragma once

#include <cstddef>
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing input data sent from client to server.
 * @details This structure contains the header information, entity ID,
 * movement deltas (dx, dy), and shooting status.
 */
struct InputData {
    HeaderData header; ///> The packet header containing type, version, and size.
    uint32_t entity;   ///> The unique identifier of the entity sending the input.
    int8_t dx;         ///> The change in x-coordinate (horizontal movement).
    int8_t dy;         ///> The change in y-coordinate (vertical movement).
    uint8_t shooting;  ///> Flag indicating whether the entity is shooting (1) or not (0).
};

#pragma pack(pop)

#pragma pack(push, 1)

/**
 * @brief Structure representing player input data with flags.
 * @details This structure contains the header information and a flags byte
 * where each bit represents a different input action (up, down, left, right, shoot).
 */
struct PlayerInputData {
    HeaderData header; ///> The packet header containing type, version, and size.
    uint8_t flags;     ///> Bitwise flags representing player inputs:
                       ///  Bit 0: Up
                       ///  Bit 1: Down
                       ///  Bit 2: Left
                       ///  Bit 3: Right
                       ///  Bit 4: Shoot
};

#pragma pack(pop)

struct PlayerInput {
    bool up = false;    ///> Flag indicating upward movement.
    bool down = false;  ///> Flag indicating downward movement.
    bool left = false;  ///> Flag indicating leftward movement.
    bool right = false; ///> Flag indicating rightward movement.
    bool shoot = false; ///> Flag indicating shooting action.
};
