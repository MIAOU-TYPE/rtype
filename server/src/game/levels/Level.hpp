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
     * @brief Definition of shooting behavior for enemies.
     */
    struct ShootDefinition {
        std::string type;               ///> Type of shooting pattern
        float cooldown = 0.f;           ///> Time between shots
        int damage = 0;                 ///> Damage per shot
        float projectileSpeed = 0.f;    ///> Speed of the projectile
        std::vector<float> angles;      ///> Shooting angles in degrees
        std::pair<float, float> muzzle; ///> Muzzle offset (x, y)
    };

    /**
     * @brief Member of an enemy group.
     */
    struct GroupMember {
        std::string enemyType; ///> Type of enemy to spawn
        float offsetX = 0.f;   ///> X offset from spawn position
        float offsetY = 0.f;   ///> Y offset from spawn position
    };

    /**
     * @brief Definition of an enemy type.
     */
    struct EnemyDefinition {
        int hp = 0;                       ///> Hit points
        float speed = 0.f;                ///> Movement speed
        float colW = 0.f;                 ///> Collision width
        float colH = 0.f;                 ///> Collision height
        unsigned int sprite;              ///> Sprite asset path
        unsigned int killScore = 0;       ///> Score awarded for killing this enemy
        ShootDefinition shoot;            ///> Shooting behavior
        bool isGroup = false;             ///> Is this a group of enemies?
        std::vector<GroupMember> members; ///> Members if this is a group
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
        std::string spawnPattern = "";      ///> Spawn pattern: line, spread (empty = random)
        float spawnY = 365.f;               ///> Y position for spawn patterns (center for line)
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
