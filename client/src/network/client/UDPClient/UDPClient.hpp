/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPClient
*/

#pragma once

#include <chrono>
#include <cstring>
#include <mutex>
#include <thread>
#include <variant>
#include <vector>
#include "ANetClient.hpp"
#include "NetWrapper.hpp"
#include "RingBuffer/RingBuffer.hpp"
#include "UDPPacket.hpp"

#ifndef WIN32
    #include <arpa/inet.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif
namespace Network
{
    /**
     * @class UDPClient
     * @brief Network client for handling UDP communication with the server.
     */
    class UDPClient : public ANetClient {
      public:
        /**
         * @brief Constructor for UDPClient.
         * Initializes the network wrapper, creates a UDP socket, and sets up the server address.
         * @throws UDPClientError if socket creation fails.
         */
        UDPClient();

        /**
         * @brief Destructor for UDPClient.
         * Closes the network connection and cleans up resources.
         */
        ~UDPClient() override;

        /**
         * @brief Receives packets from the server.
         */
        void receivePackets() override;

        /**
         * @brief Closes the network client connection.
         */
        void close() override;

        /**
         * @brief Starts the network client.
         * @throws UDPClientError if an error occurs while starting the client.
         */
        void start() override;

        /**
         * @brief Sends a packet to the server.
         * @param pkt The packet to be sent.
         * @return True if the packet was sent successfully, false otherwise.
         */
        bool sendPacket(const Net::IPacket &pkt) override;

        /**
         * @brief Pops a received packet from the internal buffer.
         * @param pkt Reference to a shared pointer where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the buffer was empty.
         */
        bool popPacket(std::shared_ptr<Net::IPacket> &pkt) override;

        /**
         * @brief Retrieves a templated packet for serialization.
         * @return A shared pointer to the templated IPacket.
         */
        std::shared_ptr<Net::IPacket> getTemplatedPacket() const noexcept override;

      private:
        Net::NetWrapper _netWrapper; ///> Network wrapper
        std::mutex _packetDataMutex; ///> Mutex for synchronizing access to packet data
        Buffer::RingBuffer<std::shared_ptr<Net::IPacket>> _ringBuffer; ///> Ring buffer to store received packets
        sockaddr_in _serverAddr{};                                     ///> Server address structure
    };

} // namespace Network