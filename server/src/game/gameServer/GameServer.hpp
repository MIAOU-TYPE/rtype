/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameServer
*/

#pragma once

#include "../../network/SessionManager/Manager/SessionManager.hpp"
#include "IMessageSink.hpp"
#include "IServer.hpp"
#include "InputSystem.hpp"
#include "MovementSystem.hpp"
#include "PacketFactory.hpp"

namespace Game
{
    /**
     * @brief The GameServer class is the core gameplay controller.
     *
     * GameServer receives network events through IMessageSink
     * and applies them to the authoritative game world. It manages:
     *
     * - player connections/disconnections
     * - player input updates
     * - high-level game state changes
     * - execution of ECS systems each frame (InputSystem, MovementSystem, etc.)
     *
     * GameServer owns its IGameWorld instance and acts as the bridge
     * between the network layer and the ECS-based game engine.
     */
    class GameServer final : public Net::IMessageSink {
      public:
        /**
         * @brief Construct a new GameServer.
         *
         * @param sessions Shared SessionManager used to resolve player addresses.
         * @param server   Network backend used to send packets to clients.
         */
        GameServer(std::shared_ptr<Net::Server::SessionManager> sessions, std::shared_ptr<Net::Server::IServer> server);

        /**
         * @brief Called when a new player connects.
         *
         * Creates a new entity in the world and associates it with the session ID.
         */
        void onPlayerConnect(int sessionId) override;

        /**
         * @brief Called when a player disconnects.
         *
         * Removes the entity associated with the session and cleans internal mappings.
         */
        void onPlayerDisconnect(int sessionId) override;

        /**
         * @brief Called when a player sends input.
         *
         * Updates the InputComponent of the associated entity.
         */
        void onPlayerInput(int sessionId, const InputComponent &msg) override;

        /**
         * @brief Called when a PING message is received.
         *
         * Sends a PONG packet back to the corresponding client.
         */
        void onPing(int sessionId) override;

        /**
         * @brief Executes one simulation step.
         *
         * Runs ECS systems in order:
         * - InputSystem (consume inputs and set velocity)
         * - MovementSystem (apply velocity to positions)
         *
         * @param dt Delta-time in seconds.
         */
        void update(float dt) const;

      private:
        std::unique_ptr<IGameWorld> _world; ///> The game world (ECS state).

        std::shared_ptr<Net::Server::SessionManager> _sessions; ///> Manages player sessions.
        std::shared_ptr<Net::Server::IServer> _server;          ///> Sends packets to clients.
        Net::Factory::PacketFactory _factory;                   ///> Builds outgoing packets.

        std::unordered_map<int, Ecs::Entity> _sessionToEntity; ///> Maps sessions to entities.
    };

} // namespace Game
