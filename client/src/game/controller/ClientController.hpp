/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientController
*/

#pragma once
#include "CommandBuffer.hpp"
#include "EventBus.hpp"
#include "IClientMessageSink.hpp"
#include "WorldCommand.hpp"

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
         * @param eventBus Shared pointer to the Eventbus for event handling
         */
        explicit ClientController(
            Command::CommandBuffer<World::WorldCommand> &buffer, std::shared_ptr<Engine::EventBus> eventBus);

        /**
         * @brief Destructor.
         */
        ~ClientController() override = default;

        /**
         * @brief Called when an ACCEPT message is received.
         * @param sessionId The session ID assigned by the server.
         */
        void onAccept(uint32_t sessionId) override;

        /**
         * @brief Called when a REJECT message is received.
         */
        void onReject() override;

        /**
         * @brief Called when a PONG message is received.
         */
        void onPong(uint32_t timestamp) override;

        /**
         * @brief Called when a GAME_OVER message is received.
         */
        void onGameOver() override;

        /**
         * @brief Called when a SNAPSHOT message is received.
         * @param serverTick The server tick at which the snapshot was taken.
         * @param data Vector of SnapshotEntity representing the game state.
         */
        void onSnapshot(uint32_t serverTick, const std::vector<SnapshotEntity> &data) override;

        /**
         * @brief Called when a SCORE message is received.
         * @param score The score received from the server.
         */
        void onScore(uint32_t score) override;

        /**
         * @brief Called when a DAMAGE_EVENT message is received.
         * @param targetId The ID of the entity that received damage.
         * @param wasKilled True if the damage killed the entity, false otherwise.
         */
        void onDamage(size_t targetId, bool wasKilled) override;

        /**
         * @brief Called when a DESTROY message is received.
         * @param entityId The ID of the entity to be destroyed.
         * @param wasKilled True if entity was killed by player (play sound), false otherwise.
         */
        void onDestroy(size_t entityId, bool wasKilled) override;

        /**
         * @brief Called when a HEALTH message is received.
         * @param currentLife The current life of the entity.
         * @param maxLife The maximum life of the entity.
         */
        void onHealth(uint16_t currentLife, uint16_t maxLife) override;

      private:
        std::reference_wrapper<Command::CommandBuffer<World::WorldCommand>>
            _commandBuffer;                          ///> Reference to the world command buffer
        std::shared_ptr<Engine::EventBus> _eventBus; ///> Shared pointer to the event bus
    };
}; // namespace Ecs
