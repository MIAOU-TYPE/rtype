
#pragma once

#include <cstdint>
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @struct AcceptData
 * @brief Structure representing the data required for a connection request.
 * Includes header information, session ID, and authentication tokens.
 */
struct AcceptData {
    HeaderData header;    ///> Header information
    uint32_t netPlayerId; ///> Unique network player identifier
};

static_assert(sizeof(AcceptData) == sizeof(HeaderData) + 4, "AcceptData layout mismatch");

#pragma pack(pop)
