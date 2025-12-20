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
    size_t id;          ///> Entity ID
    float x;            ///> X position
    float y;            ///> Y position
    std::string sprite; ///> Sprite identifier
};

#pragma pack(push, 1)

/**
 * @brief Header for a batch of snapshot entities.
 */
struct SnapshotBatchHeader {
    HeaderData header; ///> Common header data
    uint16_t count;    ///> Number of entities in the batch
};

#pragma pack(pop)

#pragma pack(push, 1)

/**
 * @brief Serialized snapshot entity data.
 */
struct SnapshotEntityData {
    uint64_t id;    ///> Entity ID
    uint32_t x;     ///> X position
    uint32_t y;     ///> Y position
    uint8_t sprite; ///> Sprite identifier
};

#pragma pack(pop)