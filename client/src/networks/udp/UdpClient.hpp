/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UdpClient
*/

#pragma once

#include <arpa/inet.h>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <queue>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include "INetworkClient.hpp"
#include "PacketSerializer.hpp"

namespace Network
{

    /**
     * @brief UDP client for R-Type game networking
     *
     * This class manages the UDP connection to the game server,
     * including sending serialized packets such as connection requests
     * and player inputs.
     */

    class UdpClient : public INetworkClient {
      public:
        /**
         * @brief Construct a new UdpClient object
         */
        UdpClient();

        /**
         * @brief Destroy the UdpClient object
         */
        ~UdpClient();

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

        /**
         * @brief Poll for incoming packets from the server
         *
         * @return std::vector<std::vector<uint8_t>> List of received packet data
         */
        std::vector<std::vector<uint8_t>> poll() override;

        /**
         * @brief Check if the client is connected to the server
         *
         * @return true if connected, false otherwise
         */
        bool isConnected() const override;

      protected:
        /**
         * @brief Internal method to handle receiving packets
         */
        void receiveLoop();

      private:
        int _socketFd;                                     ///< UDP socket file descriptor
        sockaddr_in _serverAddr;                           ///< Server address structure
        std::thread _receiverThread;                       ///< Thread for receiving packets
        std::atomic<bool> _running;                        ///< Flag to control receiver thread
        std::mutex _receiveMutex;                          ///< Mutex for thread-safe packet access
        std::queue<std::vector<uint8_t>> _receivedPackets; ///< Queue of received packets
    };

} // namespace Network