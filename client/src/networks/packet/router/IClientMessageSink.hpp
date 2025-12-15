/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IMessageSink
*/

#pragma once
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
        virtual void onEntityCreate(const EntityCreateData &data) = 0;

        /**
         * @brief Called when an ENTITY_DESTROY message is received.
         */
        virtual void onEntityDestroy(const EntityDestroyData &) = 0;

        // /**
        //  * @brief Called when a SNAPSHOT message is received.
        //  */
        virtual void onSnapshot(/**PARAM INCONNU**/) = 0;
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

        void onEntityCreate(const EntityCreateData &data) override
        {
            (void) data;
            std::cout << "Entity created!" << std::endl;
        }

        void onEntityDestroy(const EntityDestroyData &data) override
        {
            (void) data;
            std::cout << "Entity destroyed!" << std::endl;
        }

        void onSnapshot(/**PARAM INCONNU**/) override
        {
            std::cout << "Snapshot received!" << std::endl;
        }
    };
} // namespace Ecs
