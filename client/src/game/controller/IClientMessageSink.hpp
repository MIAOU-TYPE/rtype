/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IClientMessageSink
*/

#pragma once
#include <iostream>
#include <vector>
#include "SnapEntityData.hpp"

namespace Ecs
{
    /**
     * @brief Interface for handling incoming client messages.
     */
    class IClientMessageSink {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IClientMessageSink() = default;

        /**
         * @brief Called when an ACCEPT message is received.
         */
        virtual void onAccept(uint32_t sessionId) = 0;

        /**
         * @brief Called when a REJECT message is received.
         */
        virtual void onReject() = 0;

        /**
         * @brief Called when a PONG message is received.
         */
        virtual void onPong() = 0;

        /**
         * @brief Called when a GAME_OVER message is received.
         */
        virtual void onGameOver() = 0;

        /**
         * @brief Called when a SNAPSHOT message is received.
         * @param serverTick The server tick at which the snapshot was taken.
         * @param data Vector of SnapshotEntity representing the game state.
         */
        virtual void onSnapshot(uint32_t serverTick, const std::vector<SnapshotEntity> &data) = 0;

        /**
         * @brief Called when a SCORE message is received.
         * @param score The score received from the server.
         */
        virtual void onScore(uint32_t score) = 0;

        /**
         * @brief Called when a DAMAGE_EVENT message is received.
         * @param targetId The ID of the entity that received damage.
         * @param wasKilled True if the damage killed the entity, false otherwise.
         */
        virtual void onDamage(size_t targetId, bool wasKilled) = 0;

        /**
         * @brief Called when a DESTROY message is received.
         * @param entityId The ID of the entity to be destroyed.
         * @param wasKilled True if entity was killed by player (play sound), false otherwise.
         */
        virtual void onDestroy(size_t entityId, bool wasKilled) = 0;

        /**
         * @brief Called when a HEALTH message is received.
         * @param currentLife The current life of the entity.
         * @param maxLife The maximum life of the entity.
         */
        virtual void onHealth(uint16_t currentLife, uint16_t maxLife) = 0;
    };
} // namespace Ecs
