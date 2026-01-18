/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BossPhase
*/

#pragma once

#include <vector>
#include <optional>
#include "AIShoot.hpp"

namespace Ecs
{
    /**
     * @brief Structure to hold data for each boss phase
     * This structure is used to define the properties of each phase
     * of a boss, including health thresholds, attack angles,
     * fire speed multipliers, damage multipliers, and sprite IDs.
     */
    struct BossPhaseData {
        int healthThreshold;            ///> Health threshold to enter this phase
        std::vector<float> anglesToAdd; ///> Additional attack angles for this phase
        float fireSpeedMultiplier;      ///> Multiplier for fire speed in this phase
        int damageMultiplier;           ///> Multiplier for damage in this phase
        unsigned int spriteId = 0;      ///> Sprite ID for this phase
        std::optional<AIShoot::Type> shootType; ///> Optional new shoot type for this phase
        std::optional<unsigned int> projectileSpriteId; ///> Optional new projectile sprite for this phase
    };

    /** @brief Component representing the phase of a boss entity
     *
     * This component tracks the current phase of a boss, along with
     * thresholds for phase transitions and multipliers for fire rate
     * and damage. It is used to manage boss behavior as its health changes.
     */
    struct BossPhase {
        std::vector<BossPhaseData> phases; ///> Data for each boss phase
        size_t currentPhaseIndex = 0;      ///> Index of the current phase
    };
} // namespace Ecs