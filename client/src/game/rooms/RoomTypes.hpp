/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomTypes
*/

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Engine
{
    /**
     * @brief Information about a level
     */
    struct LevelInfo {
        std::string id; ///> Level identifier
        std::string displayName;    ///> Level display name
    };

    /**
     * @brief Information about a world
     */
    struct WorldEntry {
        std::string id; ///> World identifier
        std::string displayName;    ///> World display name
    };

    /**
     * @brief Levels categorized by difficulty for a world
     */
    struct WorldLevels {
        std::vector<LevelInfo> easy; ///> Easy levels
        std::vector<LevelInfo> medium;  ///> Medium levels
        std::vector<LevelInfo> hard; ///> Hard levels
    };

    /**
     * @brief Types of worlds
     */
    enum class Difficulty : std::uint8_t {
        Easy = 0,   ///> Easy difficulty
        Medium = 1, ///> Medium difficulty
        Hard = 2,   ///> Hard difficulty
    };
} // namespace Engine
