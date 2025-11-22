/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** HeaderPacket
*/

#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct HeaderPacket {
    uint8_t type;
    uint8_t version;
    uint16_t size;
};

#pragma pack(pop)
