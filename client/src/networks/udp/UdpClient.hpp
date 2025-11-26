/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UdpClient
*/

#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "../wrappers/NetWrapper.hpp"
#include "ANetworkClient.hpp"
#include "SocketConfig.hpp"
#include "UdpClientPacket.hpp"

namespace Network
{

    /**
     * @brief UDP client for R-Type game networking
     *
     * This class manages the UDP connection to the game server,
     * including sending serialized packets such as connection requests
     * and player inputs. It inherits from ANetworkClient which provides
     * common client functionality.
     */

    class UdpClient : public ANetworkClient {
      public:
        /**
         * @brief Construct a new UdpClient object
         */
        UdpClient();

        /**
         * @brief Destroy the UdpClient object
         */
        ~UdpClient() override;

        /**
         * @brief Connect to the game server
         *
         * @param ip Server IP address
         * @param port Server port number
         */
        void connectToServer(const std::string &ip, uint16_t port) override;

        /**
         * @brief Disconnect from the game server
         */
        void disconnectFromServer() override;

        /**
         * @brief Send a generic packet to the server
         *
         * @param data Serialized packet data
         */
        void sendPacket(const std::vector<uint8_t> &data) override;

      protected:
        /**
         * @brief Internal method to handle receiving packets
         */
        void receiveLoop();
    };

} // namespace Network