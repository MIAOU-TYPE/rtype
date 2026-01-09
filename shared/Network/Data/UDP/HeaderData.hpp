/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** HeaderData
*/

#pragma once
#include <cstdint>

inline constexpr uint8_t kPacketMagic[4] = {'R', 'T', 'Y', 'P'};

#pragma pack(push, 1)

/**
 * @brief Structure representing the header of a network packet.
 * @details This structure contains the type, version, and size of the packet.
 */
struct HeaderData {
    uint8_t magic[4];  ///> Magic number to identify the packet (should be 'RTYP').
    uint8_t type;      ///> The type of the packet (e.g., CONNECT, DISCONNECT, INPUT, etc.).
    uint8_t version;   ///> The version of the packet format.
    uint16_t size;     ///> The size of the entire packet, including the header.
    uint32_t sequence; ///> Sequence number of the snapshot
};

#pragma pack(pop)

static_assert(sizeof(HeaderData) == 12, "HeaderData layout mismatch");