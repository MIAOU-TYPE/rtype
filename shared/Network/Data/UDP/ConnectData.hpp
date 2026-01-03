/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ConnectData
*/

#pragma once

#include <cstdint>
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @struct ConnectData
 * @brief Structure representing the data required for a connection request.
 * Includes header information, session ID, and authentication tokens.
 */
struct ConnectData {
    HeaderData header; ///> Header information
    uint32_t sessionId; ///> Session identifier
    uint32_t tokenHi; ///> High part of the authentication token
    uint32_t tokenLo; ///> Low part of the authentication token
};

#pragma pack(pop)
