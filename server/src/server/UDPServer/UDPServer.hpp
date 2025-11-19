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
#include "socketParams.hpp"

#ifdef _WIN32
static inline void close_socket(socket_handle s)
{
    if (s != kInvalidSocket)
        ::closesocket(s);
}
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
static inline void close_socket(socket_handle s)
{
    if (s != kInvalidSocket)
        ::close(s);
}
#endif

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
        void pollOnce(int timeout) override;

      private:
        void setupSocket(const net::SocketConfig &params,
            const net::SocketOptions &optParams);        //> Sets up the UDP socket with specified parameters
        void bindSocket(net::family_t family = AF_INET); //> Binds the UDP socket to an address
    };
} // namespace Server
