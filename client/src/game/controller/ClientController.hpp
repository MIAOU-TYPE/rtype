/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientController
*/

#pragma once
#include "IClientMessageSink.hpp"
#include "WorldCommandBuffer.hpp"

namespace Ecs
{
    class ClientController final : public IClientMessageSink {
      public:
        explicit ClientController(WorldCommandBuffer &buffer);

        /**
         * @brief Destructor.
         */
        ~ClientController() override = default;

        /**
         * @brief Called when an ACCEPT message is received.
         */
        void onAccept() override;

        /**
         * @brief Called when a REJECT message is received.
         */
        void onReject() override;

        /**
         * @brief Called when a PONG message is received.
         */
        void onPong() override;

        /**
         * @brief Called when a GAME_OVER message is received.
         */
        void onGameOver() override;

        /**
         * @brief Called when an ENTITY_CREATE message is received.
         * @param data The data associated with the entity creation.
         */
        void onEntityCreate(const EntityCreate &data) override;

        /**
         * @brief Called when an ENTITY_DESTROY message is received.
         */
        void onEntityDestroy(const EntityDestroy &data) override;

        /**
         * @brief Called when a SNAPSHOT message is received.
         */
        void onSnapshot(const SnapshotEntity &data) override;

      private:
        WorldCommandBuffer &_commandBuffer;
    };
}; // namespace Ecs