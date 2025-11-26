/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntityPacket
*/

#pragma once
#include <cstdint>
#include "HeaderPacket.hpp"
#pragma pack(push, 1)

struct EntityCreatePacket {
    HeaderPacket header;
    uint64_t id;
    uint32_t x;
    uint32_t y;
    uint16_t sprite;
};

#pragma pack(pop)
