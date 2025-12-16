/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Rand
*/

#pragma once
#include <random>

/**
 * @namespace Rand
 * @brief Namespace for random number generation utilities.
 */
namespace Rand
{
    /**
     * @brief Mersenne Twister random number generator.
     */
    extern std::mt19937 rng;

    /**
     * @brief Uniform distribution for patrol velocity in the X direction.
     */
    extern std::uniform_real_distribution<float> patrolVelX;

    /**
     * @brief Uniform distribution for patrol velocity in the Y direction.
     */
    extern std::uniform_real_distribution<float> patrolVelY;

    /**
     * @brief Uniform distribution for enemy Y position.
     */
    extern std::uniform_real_distribution<float> enemyY;
} // namespace Rand
