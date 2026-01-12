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
        virtual void onAccept() = 0;

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
         * @param entity Vector of SnapshotEntity representing the game state.
         */
        virtual void onSnapshot(const std::vector<SnapshotEntity> &entity) = 0;

        /**
         * @brief Called when a SCORE message is received.
         * @param score The score received from the server.
         */
        virtual void onScore(uint32_t score) = 0;

        /**
         * @brief Called when a DAMAGE_EVENT message is received.
         * @param targetId The ID of the entity that received damage.
         * @param amount The amount of damage dealt.
         */
        virtual void onDamage(uint32_t targetId, uint16_t amount) = 0;

        /**
         * @brief Called when a DESTROY message is received.
         * @param entityId The ID of the entity to be destroyed.
         */
        virtual void onDestroy(size_t entityId) = 0;
    };
} // namespace Ecs
