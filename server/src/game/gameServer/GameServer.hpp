/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameServer
*/

#pragma once

#include "AISystem.hpp"
#include "Collision.hpp"
#include "CollisionSystem.hpp"
#include "Damage.hpp"
#include "DamageSystem.hpp"
#include "EnemySpawnSystem.hpp"
#include "GameClock.hpp"
#include "HealthSystem.hpp"
#include "IMessageSink.hpp"
#include "IServer.hpp"
#include "InputSystem.hpp"
#include "LifetimeSystem.hpp"
#include "MovementSystem.hpp"
#include "PacketFactory.hpp"
#include "SessionManager.hpp"

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
        GameServer(
            std::shared_ptr<Net::Server::ISessionManager> sessions, std::shared_ptr<Net::Server::IServer> server);

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

        /**
         * @brief Advances the game simulation based on elapsed time.
         *
         * Uses a fixed timestep approach to ensure consistent updates.
         */
        void tick();

      private:
        std::unique_ptr<IGameWorld> _world; ///> The game world (ECS state).

        std::shared_ptr<Net::Server::ISessionManager> _sessions; ///> Manages player sessions.
        std::shared_ptr<Net::Server::IServer> _server;           ///> Sends packets to clients.
        Net::Factory::PacketFactory _factory;                    ///> Builds outgoing packets.

        std::unordered_map<int, Ecs::Entity> _sessionToEntity; ///> Maps sessions to entities.

        GameClock _clock;                              ///> Tracks elapsed time for fixed timestep.
        double _accumulator = 0.0;                     ///> Accumulates time for fixed updates.
        static constexpr double FIXED_DT = 1.0 / 60.0; ///> Fixed timestep duration.
    };

} // namespace Game
