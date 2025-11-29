/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPServer
*/

#pragma once
#include <iostream>
#include <string>

#include "AServer.hpp"
#include "NetWrapper.hpp"
#include "RingBuffer.hpp"
#include "UDPPacket.hpp"
#include "socketParams.hpp"

/**
 * @namespace Server
 * @brief Contains all server-related classes and interfaces.
 * @note This namespace is used to group all server-related functionality.
 */
namespace Server
{
    /**
     * @class UDPServer
     * @brief A UDP server implementation.
     * @details This class provides methods to start, stop, and poll a UDP server.
     * It inherits from the AServer abstract base class.
     */
    class UDPServer : public AServer {
      public:
        /**
         * @brief Constructs a new UDPServer object.
         */
        UDPServer();

        /**
         * @brief Destroys the UDPServer object.
         */
        ~UDPServer() override;

        /**
         * @brief Starts the UDP server.
         * @note This method sets up the UDP socket and begins listening for incoming datagrams.
         */
        void start() override;

        /**
         * @brief Stops the UDP server.
         * @note This method shuts down the UDP socket and stops listening for incoming datagrams.
         */
        void stop() override;

        /**
         * @brief Polls the UDP server for incoming datagrams.
         * @param timeout The maximum time to wait for incoming datagrams, in milliseconds.
         */
        void readPackets() override;

        /**
         * @brief Sends a packet via the UDP server.
         * @return true if the packet was sent successfully, false otherwise.
         */
        bool sendPacket(const Net::IServerPacket &pkt) override;

        /**
         * @brief Pops a received packet from the server's packet queue.
         * @param pkt Reference to a Net::IServerPacket where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the queue was empty.
         */
        bool popPacket(Net::IServerPacket &pkt) override;

      private:
        void setupSocket(const Net::SocketConfig &params,
            const Net::SocketOptions &optParams);        ///> Sets up the UDP socket with specified parameters
        void bindSocket(Net::family_t family = AF_INET); ///> Binds the UDP socket to an address

        Buffer::RingBuffer<std::shared_ptr<Net::IServerPacket>> _rxBuffer; ///> Ring buffer to store received packets
        std::mutex _rxMutex;
    };
} // namespace Server
