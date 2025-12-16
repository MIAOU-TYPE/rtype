/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketInput
*/

#pragma once

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

/**
 * @brief High-level ECS representation of input commands.
 */
struct InputCommand {
    size_t entity; ///> Entity ID
    float dx;      ///> Change in X position
    float dy;      ///> Change in Y position
    bool shooting; ///> Shooting status
};