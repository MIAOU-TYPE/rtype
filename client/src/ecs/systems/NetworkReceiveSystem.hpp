/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkReceiveSystem
*/

#pragma once

#include "ISystem.hpp"
#include "NetClient.hpp"

namespace Ecs
{
    /**
     * @class NetworkReceiveSystem
     * @brief Handles receiving network packets for the ECS.
     *
     * This system is responsible for processing incoming
     * network packets related to entity states and actions.
     */
    class NetworkReceiveSystem : public ISystem {
      public:
        /** @brief Constructor and Destructor. */
        NetworkReceiveSystem(Game::NetClient &client);

        /** @brief Destructor. */
        ~NetworkReceiveSystem() = default;

        /**
         * @brief Updates the network receive system.
         * @param deltaTime Time elapsed since last update (in seconds).
         */
        void update(float deltaTime) override;

      private:
        Game::NetClient &_netClient;  ///> Reference to the network client
    };
} // namespace Ecs