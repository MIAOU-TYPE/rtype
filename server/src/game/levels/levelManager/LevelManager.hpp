/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelManager
*/

#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "Level.hpp"

namespace Game
{
    /**
     * @brief Manages level data and progression.
     */
    class LevelManager {
      public:
        /**
         * @brief Load level data from a JSON file.
         *
         * @param path The path to the level file.
         * @return true if loading was successful, false otherwise.
         */
        bool loadFromFile(const std::string &path);

        bool loadFromMemory(const std::string &content);

        /**
         * @brief Reset the level progression.
         */
        void reset();

        /**
         * @brief Advance the level time by a given delta time.
         *
         * @param dt The delta time to advance.
         */
        void advance(float dt);

        /**
         * @brief Get the current level data.
         *
         * @return The current level.
         */
        const Level &getCurrentLevel() const;

        /**
         * @brief Get the current time in the level.
         *
         * @return The current time.
         */
        float getTime() const;

        /**
         * @brief Determine if a wave should be spawned based on the current time.
         *
         * @param waveTime for The time of the wave to check.
         * @return true if the wave should be spawned, false otherwise.
         */
        bool shouldSpawn(float waveTime) const;

      private:
        Level _level;      ///> The current level data.
        float _time = 0.f; ///> The current time in the level.
    };
} // namespace Game
