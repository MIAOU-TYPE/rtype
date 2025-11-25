/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** INetworkClient
*/

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "NetworkException.hpp"

// Forward declaration
namespace Network
{
    class IClientPacket;
}

namespace Network
{

    /**
     * @brief Interface for network clients
     *
     * Defines the basic operations that any network client must implement,
     * such as connecting to a server, disconnecting, sending packets,
     * and polling for incoming data.
     */
    class INetworkClient {
      public:
        /**
         * @brief Destroy the INetworkClient object
         */
        virtual ~INetworkClient() = default;

        /**
         * @brief Connect to the game server
         *
         * @param ip Server IP address
         * @param port Server port number
         */
        virtual void connectToServer(const std::string &ip, uint16_t port) = 0;

        /**
         * @brief Disconnect from the game server
         */
        virtual void disconnectFromServer() = 0;

        /**
         * @brief Send a packet to the server
         *
         * @param data Serialized packet data
         */
        virtual void sendPacket(const std::vector<uint8_t> &data) = 0;

        /**
         * @brief Poll for incoming packets with metadata from the server
         *
         * @return std::vector<std::shared_ptr<IClientPacket>> List of received packets with metadata
         */
        virtual std::vector<std::shared_ptr<IClientPacket>> pollPackets() = 0;

        /**
         * @brief Check if the client is connected to the server
         *
         * @return true if connected, false otherwise
         */
        virtual bool isConnected() const = 0;
    };

} // namespace Network