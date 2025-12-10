/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Rand
*/

#include "Rand.hpp"

namespace Rand
{
    std::mt19937 rng{std::random_device{}()};

    std::uniform_real_distribution<float> patrolVelX(-10.f, 10.f);
    std::uniform_real_distribution<float> patrolVelY(-25.f, 25.f);
    std::uniform_real_distribution<float> enemyY(50.f, 450.f);
} // namespace Rand
