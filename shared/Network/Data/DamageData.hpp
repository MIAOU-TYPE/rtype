/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** DamageData
*/

#pragma once
#include "HeaderPacket.hpp"

#pragma pack(push, 1)

struct DamageData {
    HeaderPacket header;
    uint32_t id = 0;
    uint16_t amount = 0;
};

#pragma pack(pop)
