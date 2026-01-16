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
        int bulletsNbr = 0;                  ///> Number of bullets for spread type
        std::pair<float, float> muzzle;      ///> Muzzle offset (x, y)
        unsigned int projectileSpriteId = 9; ///> Sprite ID for the projectiles
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
        MovementDefinition movement;      ///> Movement behavior
    };

    /**
     * @brief Definition of an obstacle type.
     */
    struct ObstacleDefinition {
        unsigned int sprite;          ///> Sprite ID
        float colW = 0.f;             ///> Collision width
        float colH = 0.f;             ///> Collision height
        float pullStrength = 150.f;   ///> Gravity pull strength
        float damagePerSecond = 10.f; ///> Damage per second
        float radius = 200.f;         ///> Effect radius
        float innerRadius = 50.f;     ///> Damage radius
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
        std::string obstacleType = "";      ///> Optional obstacle type to spawn
        float obstacleX = 0.f;              ///> X position for obstacle
        float obstacleY = 0.f;              ///> Y position for obstacle
        int powerUps = 0;                   ///> Number of power-ups to spawn
        std::string powerUpType = "force";  ///> Type of power-up ("force" or "laser")
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
        std::string name;                                                  ///> Level name
        float duration = 0.f;                                              ///> Level duration in seconds
        std::vector<BackgroundLayer> backgroundLayers;                     ///> Background layers
        std::unordered_map<std::string, EnemyDefinition> enemyTypes;       ///> Catalog of enemy types
        std::unordered_map<std::string, ObstacleDefinition> obstacleTypes; ///> Catalog of obstacle types
        std::vector<Wave> waves;                                           ///> Waves of enemies in the level
    };
} // namespace Game
