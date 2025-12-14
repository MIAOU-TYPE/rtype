/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SnapshotSystem
*/

#pragma once

#include "Drawable.hpp"
#include "Position.hpp"
#include "SnapEntityData.hpp"
#include "World.hpp"

namespace Game
{
    /**
     * @brief System responsible for generating snapshots of the game world.
     *
     * The SnapshotSystem collects the current state of relevant entities
     * (e.g., position, drawable) and prepares a snapshot that can be sent
     * to clients for rendering.
     */
    class SnapshotSystem {
      public:
        /**
         * @brief Update the snapshot based on the current game world state.
         *
         * This method iterates over all entities with Position and Drawable
         * components and collects their data into the provided snapshot vector.
         *
         * @param world The game world to snapshot.
         * @param out The vector to populate with snapshot entities.
         */
        static void update(IGameWorld &world, std::vector<SnapshotEntity> &out);
    };
} // namespace Game
