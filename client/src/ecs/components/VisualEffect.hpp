/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** VisualEffect
*/

#pragma once

namespace Ecs
{
    /**
     * @enum EffectType
     * @brief Types of available visual effects.
     */
    enum class EffectType {
        None,
        Explosion, // Animated explosion
        Hit,       // Impact flash
        Shield,    // Shield effect
        Trail,     // Motion trail
        Spawn,     // Entity spawn
        Death      // Entity death
        // TEMPORAIRE IL Y AURA SUREMENT MOINS D'EFFETS
    };

    /**
     * @struct VisualEffect
     * @brief Component for visual effects with a lifespan.
     *
     * Used with a system that automatically destroys the entity
     * after the duration expires.
     */
    struct VisualEffect {
        EffectType type = EffectType::None;
        float duration = 1.0f;   // Total duration (seconds)
        float elapsed = 0.f;     // Elapsed time
        bool autoDestroy = true; // Destroy entity at the end
        float intensity = 1.0f;  // Effect intensity (0-1)
    };
} // namespace Ecs