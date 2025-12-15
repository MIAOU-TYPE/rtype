/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LevelManager
*/

#pragma once
#include <fstream>
#include "sstream"
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
         * @brief Load level data from a JSON string.
         *
         * @param jsonContent The JSON content as a string.
         * @return true if loading was successful, false otherwise.
         */
        bool load(const std::string &jsonContent);

        /**
         * @brief Load level data from a file.
         *
         * @param path The path to the level file.
         * @return true if loading was successful, false otherwise.
         */
        bool loadFromFile(const std::string &path);

        /**
         * @brief Reset the level progression timer.
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
         * @param waveTime Time of the wave to check.
         * @return true if the wave should be spawned, false otherwise.
         */
        bool shouldSpawn(float waveTime) const;

      private:
        Level _level;      ///> The current level data.
        float _time = 0.f; ///> The current time in the level.
    };
} // namespace Game
