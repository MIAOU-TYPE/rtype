/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameConfig
*/

#pragma once

#include <cstdint>
#include <string>

namespace Engine
{
    /**
     * @enum GameMode
     * @brief Enumeration of available game modes
     */
    enum class GameMode : uint8_t {
        Standard = 0,    ///> Standard game mode with competitive gameplay
        Coop = 1,        ///> Cooperative mode where players work together
        FriendlyFire = 2, ///> Mode allowing damage between players
        Survival = 3     ///> Survival mode with increasing difficulty
    };

    /**
     * @enum Difficulty
     * @brief Enumeration of difficulty levels
     */
    enum class Difficulty : uint8_t {
        Easy = 0,   ///> Easy difficulty
        Medium = 1, ///> Medium difficulty
        Hard = 2,   ///> Hard difficulty
    };

    /**
     * @struct ModeParameters
     * @brief Structure containing mode-specific parameters
     */
    struct ModeParameters {
        uint32_t timeLimit = 0;        ///> Time limit in seconds (0 = no limit)
        uint32_t scoreLimit = 0;       ///> Score limit to win (0 = no limit)

        bool sharedHealth = false;     ///> Whether players share health pool
        bool teamDamage = false;       ///> Whether teammates can damage each other

        float friendlyFireMultiplier = 1.0f; ///> Damage multiplier for player-to-player damage

        uint32_t waveCount = 10;       ///> Number of waves to survive
        float spawnRateMultiplier = 1.0f; ///> Enemy spawn rate multiplier
    };

    /**
     * @struct GameConfig
     * @brief Structure representing game configuration settings
     */
    struct GameConfig {
        Difficulty difficulty;         ///> Game difficulty level
        GameMode mode;                 ///> Game mode
        ModeParameters parameters;     ///> Mode-specific parameters
        std::string levelId;           ///> ID of the level to play
    };
} // namespace Engine