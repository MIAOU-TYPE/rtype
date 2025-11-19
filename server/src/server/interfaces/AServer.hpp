/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AServer
*/

#pragma once
#include <cstdint>
#include "IServer.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
using socket_handle = SOCKET;
constexpr socket_handle kInvalidSocket = INVALID_SOCKET;
#else
using socket_handle = int;
constexpr socket_handle kInvalidSocket = -1;
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
         * @brief Polls the server for events with a specified timeout.
         * @param timeout The maximum time to wait for events, in milliseconds.
         */
        virtual void pollOnce(int timeout) override = 0;

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

      protected:
        std::string _ip = "";    //> IP address the server is bound to
        uint16_t _port = 0;      //> Port number the server is listening on
        bool _isRunning = false; //> Flag indicating if the server is running

        socket_handle _socketFd = kInvalidSocket; //> Socket file descriptor
    };
} // namespace Server
