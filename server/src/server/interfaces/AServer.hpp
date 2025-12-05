/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AServer
*/

#pragma once
#include <atomic>
#include <cstdint>
#include "IServer.hpp"
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
 * @namespace Server
 * @brief Contains all server-related classes and interfaces.
 */
namespace Server
{
    /**
     * @class AServer
     * @brief Abstract base class for a server.
     * Provides common functionality for configuring and checking the status of a server.
     * Inherits from the IServer interface.
     * @note This class should be extended by specific server implementations.
     */
    class AServer : public IServer {
      public:
        /**
         * @brief Configures the server with the specified IP address and port.
         * @param ip The IP address to bind the server to.
         * @param port The port number to listen on.
         */
        void configure(const std::string &ip, uint16_t port) override;

        /**
         * @brief Starts the server.
         * @note This method should initiate the server's main loop or listening process.
         * Derived classes must implement this method.
         */
        virtual void start() override = 0;

        /**
         * @brief Stops the server.
         * @note This method should gracefully shut down the server and release resources.
         * Derived classes must implement this method.
         */
        virtual void stop() override = 0;

        /**
         * @brief Sets the server's socket to non-blocking or blocking mode.
         * @param nonBlocking True to set the socket to non-blocking mode, false for blocking mode.
         */
        void setNonBlocking(bool nonBlocking = true) override;

        /**
         * @brief Checks if the server is currently running.
         * @return True if the server is running, false otherwise.
         */
        bool isRunning() const noexcept override;

        /**
         * @brief Sets the running status of the server.
         * @param running True to set the server as running, false otherwise.
         */
        void setRunning(bool running) noexcept override;

        /**
         * @brief reads packets from the server.
         */
        virtual void readPackets() override = 0;

        /**
         * @brief Sends a packet through the server.
         * @param pkt The packet to be sent.
         * @return True if the packet was sent successfully, false otherwise.
         */
        virtual bool sendPacket(const Net::IPacket &pkt) override = 0;

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
         * @brief Pops a received packet from the server's packet queue.
         * @param pkt Reference to a Net::IPacket where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the queue was empty.
         */
        virtual bool popPacket(std::shared_ptr<Net::IPacket> &pkt) override = 0;

      protected:
        std::string _ip = "";                ///> IP address the server is bound to
        uint16_t _port = 0;                  ///> Port number the server is listening on
        std::atomic<bool> _isRunning{false}; ///> Atomic flag indicating if the server is running

        socketHandle _socketFd = kInvalidSocket; ///> Socket file descriptor
    };
} // namespace Server
