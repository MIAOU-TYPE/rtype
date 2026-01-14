/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GravityField
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct GravityField
     * @brief Component that defines a gravity field that pulls entities toward it.
     *
     * This component is used to create obstacles that attract nearby entities
     * and deal damage to them when they get too close.
     */
    struct GravityField {
        float pullStrength = 150.f;   ///> Force of attraction in units/second
        float damagePerSecond = 10.f; ///> Damage inflicted per second
        float radius = 200.f;         ///> Radius of effect
        float innerRadius = 50.f;     ///> Radius where damage is applied
        float lifetime = 15.0f;       ///> Duration in seconds before disappearing
    };
} // namespace Ecs
