/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IClientMessageSink
*/

#pragma once
#include <iostream>
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
         * @brief Called when an ENTITY_CREATE message is received.
         * @param data The data associated with the entity creation.
         */
        virtual void onEntityCreate(const EntityCreate &data) = 0;

        /**
         * @brief Called when an ENTITY_DESTROY message is received.
         */
        virtual void onEntityDestroy(const EntityDestroy &data) = 0;

        /**
         * @brief Called when a SNAPSHOT message is received.
         */
        virtual void onSnapshot(const std::vector<SnapshotEntity> &entity) = 0;
    };
} // namespace Ecs