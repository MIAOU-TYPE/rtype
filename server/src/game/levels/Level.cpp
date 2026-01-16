/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Level
*/

#include "Level.hpp"

namespace Game
{
    DifficultyModifiers DifficultyModifiers::fromDifficulty(Difficulty difficulty)
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
} // namespace Game
