/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BossPhase
*/

#pragma once

#include <vector>

namespace Ecs
{
    /** @brief Component representing the phase of a boss entity
     *
     * This component tracks the current phase of a boss, along with
     * thresholds for phase transitions and multipliers for fire rate
     * and damage. It is used to manage boss behavior as its health changes.
     */
    struct BossPhase {
        enum class Phase {
            Phase1, ///> Initial phase of the boss
            Phase2, ///> Second phase of the boss
            Phase3  ///> Final phase of the boss
        };
        Phase currentPhase = Phase::Phase1;                   ///> Current phase of the boss
        std::vector<int> phaseThresholds = {2500, 1000, 200}; ///> Health thresholds for phase transitions
        float fireSpeedMultiplier = 1.5f;                     ///> Multiplier for fire rate in current phase
        int damageMultiplier = 2;                             ///> Multiplier for damage in current phase
    };
} // namespace Ecs