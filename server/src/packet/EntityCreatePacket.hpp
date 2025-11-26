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
    uint64_t id = 0;
    uint32_t x = 0.0f;
    uint32_t y = 0.0f;
    uint16_t sprite = 0;
};

#pragma pack(pop)
