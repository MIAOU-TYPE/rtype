/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** EntityDestroyPacket
*/

#pragma once
#include <cstdint>
#include "HeaderPacket.hpp"
#pragma pack(push, 1)

struct EntityDestroyPacket {
    HeaderPacket header;
    uint64_t id = 0;
};

#pragma pack(pop)
