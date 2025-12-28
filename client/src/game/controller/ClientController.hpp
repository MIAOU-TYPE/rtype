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
    /**
     * @class ClientController
     * @brief Handles incoming client messages and translates them into world commands.
     * This class implements the IClientMessageSink interface to process various
     * types of messages received from the server.
     */
    class ClientController final : public IClientMessageSink {
      public:
        /**
         * @brief Constructor.
         * @param buffer Reference to the WorldCommandBuffer to push commands into.
         */
        explicit ClientController(Engine::WorldCommandBuffer &buffer);

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
         * @brief Called when a SNAPSHOT message is received.
         */
        void onSnapshot(const std::vector<SnapshotEntity> &data) override;

        /**
         * @brief Called when a SCORE message is received.
         * @param score The score received from the server.
         */
        void onScore(uint32_t score) override;

      private:
        std::reference_wrapper<Engine::WorldCommandBuffer> _commandBuffer; ///> Reference to the world command buffer
    };
}; // namespace Ecs