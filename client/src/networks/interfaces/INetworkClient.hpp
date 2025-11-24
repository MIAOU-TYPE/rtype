/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** INetworkClient
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "NetworkException.hpp"

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
         * @brief Poll for incoming packets from the server
         *
         * @return std::vector<std::vector<uint8_t>> List of received packet data
         */
        virtual std::vector<std::vector<uint8_t>> poll() = 0;

        /**
         * @brief Check if the client is connected to the server
         *
         * @return true if connected, false otherwise
         */
        virtual bool isConnected() const = 0;
    };

} // namespace Network