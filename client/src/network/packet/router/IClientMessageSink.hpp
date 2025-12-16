/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IMessageSink
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
        virtual void onSnapshot(const SnapshotEntity &entity) = 0;
    };

    class testSink : public IClientMessageSink {
      public:
        void onAccept() override
        {
            std::cout << "Accepted by server!" << std::endl;
        }

        void onReject() override
        {
            std::cout << "Rejected by server!" << std::endl;
        }

        void onPong() override
        {
            std::cout << "Pong!" << std::endl;
        }

        void onGameOver() override
        {
            std::cout << "Game over!" << std::endl;
        }

        void onEntityCreate(const EntityCreate &data) override
        {
            std::cout << "Entity created: ID=" << data.id << " X=" << data.x << " Y=" << data.y << std::endl;
        }

        void onEntityDestroy(const EntityDestroy &data) override
        {
            std::cout << "Entity created: ID=" << data.id << std::endl;
        }

        void onSnapshot(const SnapshotEntity &entity) override
        {
            std::cout << "Snapshot Entity: ID=" << entity.entity << " X=" << entity.x << " Y=" << entity.y
                      << " Sprite=" << entity.sprite << std::endl;
        }
    };
} // namespace Ecs