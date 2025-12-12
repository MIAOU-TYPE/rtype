/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SnapEntityData
*/

#pragma once
#include <cstdint>
#include <string>
#include "HeaderData.hpp"

/**
 * @brief High-level ECS snapshot representation.
 */
struct SnapshotEntity {
    size_t entity;
    float x;
    float y;
    std::string sprite;
};

/**
 * @brief Network-ready, packed structure.
 * This structure should be serialized directly.
 */
#pragma pack(push, 1)

struct SnapshotEntityData {
    HeaderData header;
    uint64_t entity;
    uint32_t x;
    uint32_t y;
    char sprite[32];
};

#pragma pack(pop)
