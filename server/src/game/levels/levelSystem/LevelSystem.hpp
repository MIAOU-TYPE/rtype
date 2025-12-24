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
         */
        static void update(IGameWorld &world, LevelManager &lvl, float dt);

      private:
        /**
         * @brief Handle the spawning of waves based on the current level time.
         *
         * @param world The game world to spawn enemies in.
         * @param lvl The level manager containing level data.
         */
        static void handleWaves(IGameWorld &world, const LevelManager &lvl);

        /**
         * @brief Spawn all enemy groups in a given wave.
         *
         * @param world The game world to spawn enemies in.
         * @param level The current level data.
         * @param wave The wave to spawn.
         */
        static void spawnWave(IGameWorld &world, const Level &level, const Wave &wave);

        /**
         * @brief Spawn a single enemy based on the enemy definition.
         *
         * @param world The game world to spawn the enemy in.
         * @param def The enemy definition.
         */
        static void spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def);

        static std::vector<bool> _spawned; ///> Tracks which waves have been spawned
    };
} // namespace Game
