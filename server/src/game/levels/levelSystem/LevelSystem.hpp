/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelSystem
*/

#pragma once

#include <cstdlib>
#include <iostream>
#include "AIBrain.hpp"
#include "AIShoot.hpp"
#include "Attack.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"
#include "Drawable.hpp"
#include "Health.hpp"
#include "IGameWorld.hpp"
#include "KillScore.hpp"
#include "LevelManager.hpp"
#include "Position.hpp"
#include "Rand.hpp"
#include "Target.hpp"
#include "Velocity.hpp"
#include <unordered_map>

namespace Game
{
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
         */
        static void update(IGameWorld &world, LevelManager &lvl, float dt, std::vector<bool> &spawned);

      private:
        /**
         * @brief Handle the spawning of waves based on the current level time.
         *
         * @param world The game world to spawn enemies in.
         * @param lvl The level manager containing level data.
         * @param spawned Vector tracking which waves have been spawned.
         */
        static void handleWaves(IGameWorld &world, const LevelManager &lvl, std::vector<bool> &spawned);

        /**
         * @brief Spawn all enemy groups in a given wave.
         *
         * @param world The game world to spawn enemies in.
         * @param level The current level data.
         * @param wave The wave to spawn.
         */
        static void spawnWave(IGameWorld &world, const Level &level, const Wave &wave);

        /**
         * @brief Spawn a group of enemies at relative positions.
         *
         * @param world The game world to spawn enemies in.
         * @param level The current level data.
         * @param groupDef The group definition containing member positions.
         * @param pattern Spawn pattern for the group base position.
         * @param centerY Center Y position for pattern spawning.
         */
        static void spawnEnemyGroup(IGameWorld &world, const Level &level, const EnemyDefinition &groupDef,
            const std::string &pattern, float centerY);

        /**
         * @brief Spawn a single enemy at a specific position.
         *
         * @param world The game world to spawn the enemy in.
         * @param def The enemy definition.
         * @param x X coordinate to spawn at.
         * @param y Y coordinate to spawn at.
         */
        static void spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def, float x, float y);
    };
} // namespace Game
