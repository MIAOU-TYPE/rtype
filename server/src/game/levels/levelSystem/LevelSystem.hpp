/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelSystem
*/

#pragma once

#include <cmath>
#include <cstdlib>
#include "AIBrain.hpp"
#include "AIShoot.hpp"
#include "Background.hpp"
#include "BossPhase.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"
#include "Drawable.hpp"
#include "GameConstants.hpp"
#include "GravityField.hpp"
#include "Health.hpp"
#include "IGameWorld.hpp"
#include "KillScore.hpp"
#include "Level.hpp"
#include "LevelManager.hpp"
#include "MovementPattern.hpp"
#include "Position.hpp"
#include "PowerUp.hpp"
#include "PowerUpType.hpp"
#include "Rand.hpp"
#include "Target.hpp"
#include "Velocity.hpp"
#include "WeaponConfig.hpp"
#include <unordered_map>

namespace Game
{

    constexpr float COLLISION_SCALE = 1.7f; ///> Scale factor for enemy collision boxes

    /**
     * @brief System responsible for managing level progression and enemy spawning.
     */
    class LevelSystem {
      public:
        /**
         * @brief Update the level system for the given game world and level manager.
         *
         * @param world The game world to update.
         * @param lvl The level manager to use for level data.
         * @param dt The delta time since the last update.
         * @param spawned Vector tracking which waves have been spawned.
         * @param modifiers Difficulty modifiers to apply to enemies.
         */
        static void update(IGameWorld &world, LevelManager &lvl, float dt, std::vector<bool> &spawned,
            const DifficultyModifiers &modifiers = {});

        /**
         * @brief Spawn background layers for the level (creates 2 tiles for seamless scrolling).
         *
         * @param world The game world to spawn backgrounds in.
         * @param level The current level data containing background definitions.
         */
        static void spawnBackgrounds(IGameWorld &world, const Level &level);

      private:
        /**
         * @brief Handle the spawning of waves based on the current level time.
         *
         * @param world The game world to spawn enemies in.
         * @param lvl The level manager containing level data.
         * @param spawned Vector tracking which waves have been spawned.
         * @param modifiers Difficulty modifiers to apply to enemies.
         */
        static void handleWaves(IGameWorld &world, const LevelManager &lvl, std::vector<bool> &spawned,
            const DifficultyModifiers &modifiers);

        /**
         * @brief Spawn all enemy groups in a given wave.
         *
         * @param world The game world to spawn enemies in.
         * @param level The current level data.
         * @param wave The wave to spawn.
         * @param modifiers Difficulty modifiers to apply to enemies.
         */
        static void spawnWave(
            IGameWorld &world, const Level &level, const Wave &wave, const DifficultyModifiers &modifiers);

        /**
         * @brief Spawn a group of enemies at relative positions.
         *
         * @param world The game world to spawn enemies in.
         * @param level The current level data.
         * @param groupDef The group definition containing member positions.
         * @param pattern Spawn pattern for the group base position.
         * @param centerY Center Y position for pattern spawning.
         * @param modifiers Difficulty modifiers to apply to enemies.
         */
        static void spawnEnemyGroup(IGameWorld &world, const Level &level, const EnemyDefinition &groupDef,
            const std::string &pattern, const float centerY, const DifficultyModifiers &modifiers);

        /**
         * @brief Spawn a single enemy at a specific position.
         *
         * @param world The game world to spawn the enemy in.
         * @param def The enemy definition.
         * @param x X coordinate to spawn at.
         * @param y Y coordinate to spawn at.
         * @param modifiers Difficulty modifiers to apply to the enemy.
         */
        static void spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def, const float x, const float y,
            const DifficultyModifiers &modifiers);

        /**
         * @brief Spawn an obstacle at a specific position.
         *
         * @param world The game world to spawn the obstacle in.
         * @param def The obstacle definition.
         * @param x X coordinate to spawn at.
         * @param y Y coordinate to spawn at.
         */
        static void spawnObstacle(IGameWorld &world, const ObstacleDefinition &def, float x, float y);

        /**
         * @brief Create a single background entity with given parameters.
         *
         * @param world The game world to create the background in.
         * @param layer The background layer definition.
         * @param xPosition The x position for the background tile.
         * @param scaledWidth The scaled width of the background tile.
         * @param tileIndex The tile index (0 or 1) for seamless scrolling.
         */
        static void createBackgroundEntity(
            IGameWorld &world, const BackgroundLayer &layer, float xPosition, float scaledWidth, int tileIndex);

        /**
         * @brief Spawn a power-up.
         *
         * @param world The game world to spawn the power-up in.
         * @param type The type of power-up to spawn ("force", "laser" or "shield").
         */
        static void spawnPowerUp(IGameWorld &world, const std::string &type = "force");
    };
} // namespace Game
