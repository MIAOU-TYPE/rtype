/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkSendSystem
*/

#pragma once

#include "ISystem.hpp"
#include "NetClient.hpp"
#include "Registry.hpp"
#include "InputComponent.hpp"

namespace Ecs
{
    /**
     * @class NetworkSystem
     * @brief Handles network communication for the ECS.
     *
     * This system is responsible for sending and receiving
     * network packets related to entity states and actions.
     */
    class NetworkSendSystem : public ISystem {
      public:
        /** 
         * @brief Constructor.
         * @param registry Shared pointer to the ECS registry.
         * @param netClient Shared pointer to the network client.
         */
        NetworkSendSystem(std::shared_ptr<Registry> registry, std::shared_ptr<Game::NetClient> netClient);

        /** @brief Destructor. */
        ~NetworkSendSystem() = default;

        /** 
         * @brief Updates the network system.
         * @param deltaTime Time elapsed since last update (in seconds).
         */
        void update(float deltaTime) override;    

      private:
        std::shared_ptr<Registry> _registry; ///> Shared pointer to the ECS registry.
        std::shared_ptr<Game::NetClient> _netClient; ///> Shared pointer to the network client.
    };
} // namespace Ecs