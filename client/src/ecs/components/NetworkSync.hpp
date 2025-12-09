/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkSync
*/

#pragma once
#include <cstdint>

namespace Ecs
{
    /**
     * @struct NetworkSync
     * @brief Component for synchronizing entity state over the network.
     * This component manages network IDs and interpolation data
     * to ensure smooth movement of entities based on server snapshots.
     */
    struct NetworkSync {
        uint32_t networkId = 0; ///> Unique network ID of the entity

        float prevX = 0.f; ///> Position at snapshot N-1
        float prevY = 0.f; ///> Position at snapshot N-1
        float targetX = 0.f; ///> Position at snapshot N
        float targetY = 0.f; ///> Position at snapshot N

        float interpolationFactor = 0.f; ///> Progression (0.0 → 1.0)
        float lastSnapshotTime = 0.f;    ///> Timestamp of the last snapshot
        float snapshotInterval = 0.05f;  ///> Expected frequency (20 Hz = 0.05s)

        bool authoritative = false; ///> If true, no interpolation (local entity)
    };
} // namespace Ecs