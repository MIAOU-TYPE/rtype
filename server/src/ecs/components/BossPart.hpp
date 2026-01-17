/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BossPart
*/

#pragma once

#include "Entity.hpp"

namespace Ecs
{
    /**
     * @brief Component representing hitbox zones for a boss entity
     * This component defines multiple hitbox zones for a boss,
     * each with its own damage multiplier. This allows for bosses
     * to have weak points or areas that take reduced damage.
     * */
    struct BossPart {
        Ecs::Entity bossEntity; ///> The main boss entity this part belongs to
        float damageMultiplier; ///> Damage multiplier for this part
    };
} // namespace Ecs