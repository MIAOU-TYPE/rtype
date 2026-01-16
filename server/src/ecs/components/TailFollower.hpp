/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** TailFollower
*/

#pragma once

#include "Entity.hpp"

namespace Ecs
{
    /**
     * @struct TailFollower
     * @brief Component that marks an entity as a tail follower.
     *
     * Entities with this component will follow the tail of another entity,
     * typically used for boss entities with segmented tails.
     */
    struct TailFollower {
        Ecs::Entity bossEntity;    ///< The boss entity this tail segment is associated with
        Ecs::Entity leaderEntity;    ///< The entity that this tail segment follows
        float followDistance = 50.f; ///< The distance to maintain from the leader entity
        float followSpeed = 100.f;   ///< The speed at which the tail segment follows the leader
    };
} // namespace Ecs