/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TargetingSystem
*/

#pragma once

#include <cmath>
#include <limits>
#include "Controllable.hpp"
#include "IGameWorld.hpp"
#include "Position.hpp"
#include "Target.hpp"

namespace Game
{
    /**
     * @brief System that updates entity targets
     */
    class TargetingSystem {
      public:
        static void update(IGameWorld &world); ///> Update the targeting system
    };
} // namespace Game
