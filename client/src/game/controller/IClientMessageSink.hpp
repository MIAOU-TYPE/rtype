/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IClientMessageSink
*/

#pragma once
#include <iostream>
#include <vector>
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
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
         */
        virtual void onSnapshot(const std::vector<SnapshotEntity> &entity) = 0;
    };
} // namespace Ecs