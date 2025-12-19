/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetClient
*/

#pragma once
#include <atomic>
#include <cstdint>
#include "INetClient.hpp"
#include "NetWrapper.hpp"
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

/**
 * @namespace Network
 * @brief Contains all NetClient-related classes and interfaces.
 */
namespace Network
{
    /**
     * @class ANetClient
     * @brief Abstract base class for a NetClient.
     * Provides common functionality for configuring and checking the status of a NetClient.
     * Inherits from the INetClient interface.
     * @note This class should be extended by specific NetClient implementations.
     */
    class ANetClient : public INetClient {
      public:
        /**
         * @brief Configures the NetClient with the specified IP address and port.
         * @param ip The IP address to bind the NetClient to.
         * @param port The port number to listen on.
         */
        void configure(const std::string &ip, int32_t port) override;

        /**
         * @brief Starts the NetClient.
         * @note This method should initiate the NetClient's main loop or listening process.
         * Derived classes must implement this method.
         */
        void start() override = 0;

        /**
         * @brief Stops the NetClient.
         * @note This method should gracefully shut down the NetClient and release resources.
         * Derived classes must implement this method.
         */
        void close() override = 0;

        /**
         * @brief Sets the NetClient's socket to non-blocking or blocking mode.
         * @param nonBlocking True to set the socket to non-blocking mode, false for blocking mode.
         */
        void setNonBlocking(bool nonBlocking) override;

        /**
         * @brief Checks if the NetClient is currently running.
         * @return True if the NetClient is running, false otherwise.
         */
        bool isRunning() const noexcept override;

        /**
         * @brief Sets the running status of the NetClient.
         * @param running True to set the NetClient as running, false otherwise.
         */
        void setRunning(bool running) noexcept override;

        /**
         * @brief reads packets from the NetClient.
         */
        void receivePackets() override = 0;

        /**
         * @brief Sends a packet through the NetClient.
         * @param pkt The packet to be sent.
         * @return True if the packet was sent successfully, false otherwise.
         */
        bool sendPacket(const Net::IPacket &pkt) override = 0;

        /**
         * @brief Checks if the stored IP address is valid.
         * @return True if the stored IP address is valid, false otherwise.
         */
        bool isStoredIpCorrect() const noexcept override;

        /**
         * @brief Checks if the stored port number is valid.
         * @return True if the stored port number is valid, false otherwise.
         */
        bool isStoredPortCorrect() const noexcept override;

        /**
         * @brief Pops a received packet from the NetClient's packet queue.
         * @param pkt Reference to a Network::IPacket where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the queue was empty.
         */
        bool popPacket(std::shared_ptr<Net::IPacket> &pkt) override = 0;

        /**
         * @brief Retrieves a templated packet for serialization.
         * @return A shared pointer to the templated IPacket.
         */
        std::shared_ptr<Net::IPacket> getTemplatedPacket() const noexcept override = 0;

      protected:
        std::string _ip = "";                ///> IP address the NetClient is bound to
        int32_t _port = 0;                   ///> Port number the NetClient is listening on
        std::atomic<bool> _isRunning{false}; ///> Atomic flag indicating if the NetClient is running

        socketHandle _socketFd = kInvalidSocket; ///> Socket file descriptor
    };
} // namespace Network
