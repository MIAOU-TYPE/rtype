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
#include "GameConfig.hpp"

namespace Engine
{
    /**
     * @brief Information about a level
     */
    struct LevelInfo {
        std::string id;          ///> Level identifier
        std::string displayName; ///> Level display name
        std::string path;        ///> Path to the level file on server
    };

    /**
     * @brief Information about a world
     */
    struct WorldEntry {
        std::string id;          ///> World identifier
        std::string displayName; ///> World display name
    };

    /**
     * @brief Levels for a world
     */
    struct WorldLevels {
        std::vector<LevelInfo> levels; ///> All levels in this world
    };
} // namespace Engine
