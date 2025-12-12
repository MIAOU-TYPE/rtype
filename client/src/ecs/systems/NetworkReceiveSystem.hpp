/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkReceiveSystem
*/

#pragma once

#include <memory>
#include "CommandBuffer.hpp"
#include "ISystem.hpp"
#include "NetClient.hpp"

namespace Ecs
{
    /**
     * @class NetworkReceiveSystem
     * @brief Handles receiving network packets for the ECS.
     *
     * This system is responsible for processing incoming
     * network packets from the CommandBuffer. The packets are
     * received by a dedicated network thread and pushed into
     * the CommandBuffer for thread-safe consumption here.
     */
    class NetworkReceiveSystem : public ISystem {
      public:
        /**
         * @brief Constructor.
         * @param client Reference to the network client.
         * @param commandBuffer Shared pointer to the network command buffer.
         */
        NetworkReceiveSystem(
            Game::NetClient &client, std::shared_ptr<Command::CommandBuffer<Game::NetworkCommand>> commandBuffer);

        /** @brief Destructor. */
        ~NetworkReceiveSystem() override = default;

        /**
         * @brief Updates the network receive system.
         * Processes all pending packets from the CommandBuffer.
         * @param deltaTime Time elapsed since last update (in seconds).
         */
        void update(float deltaTime) override;

      private:
        /**
         * @brief Handles a received network command.
         * @param cmd The network command containing packet data.
         */
        void handleCommand(const Game::NetworkCommand &cmd);

        Game::NetClient &_netClient; ///> Reference to the network client
        std::shared_ptr<Command::CommandBuffer<Game::NetworkCommand>> _commandBuffer; ///> Command buffer for packets
    };
} // namespace Ecs