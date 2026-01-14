/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpType
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @enum PowerUpTypeEnum
     * @brief Different types of power-ups available in the game
     */
    enum class PowerUpTypeEnum {
        Standard, ///> Standard power-up with attached weapon
        Laser     ///> Laser power-up that fires a continuous beam for 5 seconds
    };

    /**
     * @struct PowerUpType
     * @brief Component defining the type of a power-up entity
     */
    struct PowerUpType {
        PowerUpTypeEnum type = PowerUpTypeEnum::Standard; ///> The type of this power-up
    };
} // namespace Ecs
