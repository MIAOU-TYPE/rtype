/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DamagePacket
*/

#pragma once
#include "HeaderPacket.hpp"

#pragma pack(push, 1)

struct DamagePacket {
    HeaderPacket header;
    uint32_t id;
    uint16_t amount;
};

#pragma pack(pop)
