/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Level
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Game
{
    /**
     * @brief Definition of an enemy type.
     */
    struct EnemyDefinition {
        int hp = 0;         ///> Hit points
        float speed = 0.f;  ///> Movement speed
        float colW = 0.f;   ///> Collision width
        float colH = 0.f;   ///> Collision height
        int sprite;         ///> Sprite asset path
    };

    /**
     * @brief Group of enemies to spawn in a wave.
     */
    struct WaveEnemyGroup {
        std::string type; ///> Enemy type identifier
        int count = 0;    ///> Number of enemies to spawn
    };

    /**
     * @brief A wave of enemies in a level.
     */
    struct Wave {
        float time = 0.f;                   ///> Time to spawn the wave
        std::vector<WaveEnemyGroup> groups; ///> Enemy groups in the wave
    };

    /**
     * @brief Definition of a game level.
     */
    struct Level {
        std::string name;                                            ///> Level name
        float duration = 0.f;                                        ///> Level duration in seconds
        std::unordered_map<std::string, EnemyDefinition> enemyTypes; ///> Catalog of enemy types
        std::vector<Wave> waves;                                     ///> Waves of enemies in the level
    };
} // namespace Game
