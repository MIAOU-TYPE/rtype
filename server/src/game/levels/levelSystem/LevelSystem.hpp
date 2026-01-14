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
#include "Background.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"
#include "Drawable.hpp"
#include "Health.hpp"
#include "IGameWorld.hpp"
#include "KillScore.hpp"
#include "Level.hpp"
#include "LevelManager.hpp"
#include "MovementPattern.hpp"
#include "Position.hpp"
#include "Rand.hpp"
#include "Target.hpp"
#include "Velocity.hpp"
#include "WeaponConfig.hpp"
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
         * @param modifiers Difficulty modifiers to apply to enemies.
         */
        static void update(IGameWorld &world, LevelManager &lvl, float dt, std::vector<bool> &spawned, const DifficultyModifiers &modifiers = {});

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
        static void handleWaves(IGameWorld &world, const LevelManager &lvl, std::vector<bool> &spawned, const DifficultyModifiers &modifiers);

        /**
         * @brief Spawn all enemy groups in a given wave.
         *
         * @param world The game world to spawn enemies in.
         * @param level The current level data.
         * @param wave The wave to spawn.
         * @param modifiers Difficulty modifiers to apply to enemies.
         */
        static void spawnWave(IGameWorld &world, const Level &level, const Wave &wave, const DifficultyModifiers &modifiers);

        /**
         * @brief Spawn a single enemy based on the enemy definition.
         *
         * @param world The game world to spawn the enemy in.
         * @param def The enemy definition.
         * @param modifiers Difficulty modifiers to apply to the enemy.
         */
        static void spawnSingleEnemy(IGameWorld &world, const EnemyDefinition &def, const DifficultyModifiers &modifiers);

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
    };
} // namespace Game
