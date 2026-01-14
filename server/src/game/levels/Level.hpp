/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Level
*/

#pragma once

#include <string>
#include <vector>
#include "../../../shared/Network/Data/GameConfig.hpp"
#include <unordered_map>

namespace Game
{
    /**
     * @brief Difficulty modifiers that affect enemy properties
     */
    struct DifficultyModifiers {
        float enemyHpMultiplier = 1.0f;         ///> Multiplier for enemy hit points
        float enemyDamageMultiplier = 1.0f;     ///> Multiplier for enemy damage
        float enemySpawnRateMultiplier = 1.0f;  ///> Multiplier for enemy spawn rate (lower = faster spawns)
        float projectileSpeedMultiplier = 1.0f; ///> Multiplier for projectile speeds
        float enemySpeedMultiplier = 1.0f;      ///> Multiplier for enemy movement speed
        float enemyScoreMultiplier = 1.0f;      ///> Multiplier for kill score rewards

        /**
         * @brief Create difficulty modifiers based on difficulty level
         * @param difficulty The difficulty level
         * @return DifficultyModifiers configured for the given difficulty
         */
        static DifficultyModifiers fromDifficulty(Engine::Difficulty difficulty)
        {
            DifficultyModifiers mods;

            switch (difficulty) {
                case Engine::Difficulty::Easy:
                    mods.enemyHpMultiplier = 0.8f;
                    mods.enemyDamageMultiplier = 0.7f;
                    mods.enemySpawnRateMultiplier = 1.2f;
                    mods.projectileSpeedMultiplier = 0.9f;
                    mods.enemySpeedMultiplier = 0.9f;
                    mods.enemyScoreMultiplier = 0.8f;
                    break;

                case Engine::Difficulty::Medium: break;

                case Engine::Difficulty::Hard:
                    mods.enemyHpMultiplier = 1.5f;
                    mods.enemyDamageMultiplier = 1.3f;
                    mods.enemySpawnRateMultiplier = 0.8f;
                    mods.projectileSpeedMultiplier = 1.2f;
                    mods.enemySpeedMultiplier = 1.1f;
                    mods.enemyScoreMultiplier = 1.5f;
                    break;
            }

            return mods;
        }
    };

    /**
     * @brief Definition of movement behavior for enemies.
     */
    struct MovementDefinition {
        std::string type = "straight"; ///> Type of movement pattern
        float amplitude = 50.f;        ///> Amplitude for movement patterns
        float frequency = 0.5f;        ///> Frequency for movement patterns
    };

    /**
     * @brief Definition of shooting behavior for enemies.
     */
    struct ShootDefinition {
        std::string type;                    ///> Type of shooting pattern
        float cooldown = 0.f;                ///> Time between shots
        int damage = 0;                      ///> Damage per shot
        float projectileSpeed = 0.f;         ///> Speed of the projectile
        std::vector<float> angles;           ///> Shooting angles in degrees
        std::pair<float, float> muzzle;      ///> Muzzle offset (x, y)
        unsigned int projectileSpriteId = 9; ///> Sprite ID for the projectiles
    };

    /**
     * @brief Definition of an enemy type.
     */
    struct EnemyDefinition {
        int hp = 0;                  ///> Hit points
        float speed = 0.f;           ///> Movement speed
        float colW = 0.f;            ///> Collision width
        float colH = 0.f;            ///> Collision height
        unsigned int sprite;         ///> Sprite asset path
        unsigned int killScore = 0;  ///> Score awarded for killing this enemy
        ShootDefinition shoot;       ///> Shooting behavior
        MovementDefinition movement; ///> Movement behavior
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
     * @brief Definition of a background layer.
     */
    struct BackgroundLayer {
        unsigned int spriteId; ///> Sprite asset identifier
        float scrollSpeed;     ///> Scrolling speed of the layer
        float tileWidth;       ///> Width of a single background tile
        float tileHeight;      ///> Height of a single background tile
        int depth = 0;         ///> Depth layer (0 = farthest)
    };

    /**
     * @brief Definition of a game level.
     */
    struct Level {
        std::string name;                                            ///> Level name
        float duration = 0.f;                                        ///> Level duration in seconds
        std::vector<BackgroundLayer> backgroundLayers;               ///> Background layers
        std::unordered_map<std::string, EnemyDefinition> enemyTypes; ///> Catalog of enemy types
        std::vector<Wave> waves;                                     ///> Waves of enemies in the level
    };
} // namespace Game
