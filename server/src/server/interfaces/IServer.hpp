/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IServer
*/

#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
#include "IPacket.hpp"

/**
 * @namespace Server
 * @brief Contains all server-related classes and interfaces.
 */
namespace Net::Server
{
    /**
     * @class ServerError
     * @brief Exception class for server-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class ServerError : public std::exception {
      public:
        /**
         * @brief Constructor for ServerError.
         * @param message The error message to be associated with the exception.
         */
        explicit ServerError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @interface IServer
     * @brief Interface for a server.
     * Defines the essential methods for configuring, starting, stopping,
     * and checking the status of a server.
     * @note This interface should be implemented by any server class.
     */
    class IServer {
      public:
        /**
         * @brief Virtual destructor for IServer interface.
         * Ensures proper cleanup of derived classes.
         * @note Always declare destructors in interfaces as virtual.
         */
        virtual ~IServer() = default;

        /**
         * @brief Configures the server with the specified IP address and port.
         * @param ip The IP address to bind the server to.
         * @param port The port number to listen on.
         */
        virtual void configure(const std::string &ip, int32_t port) = 0;

        /**
         * @brief Starts the server.
         * @note This method should initiate the server's main loop or listening process.
         */
        virtual void start() = 0;

        /**
         * @brief Stops the server.
         * @note This method should gracefully shut down the server and release resources.
         */
        virtual void stop() noexcept = 0;

        /**
         * @brief Sets the server's socket to non-blocking or blocking mode.
         * @param nonBlocking True to set the socket to non-blocking mode, false for blocking mode.
         */
        virtual void setNonBlocking(bool nonBlocking) = 0;

        /**
         * @brief Checks if the server is currently running.
         * @return True if the server is running, false otherwise.
         */
        virtual bool isRunning() const noexcept = 0;

        /**
         * @brief Sets the running status of the server.
         * @param running True to set the server as running, false otherwise.
         */
        virtual void setRunning(bool running) noexcept = 0;

        /**
         * @brief reads packets from the server.
         */
        virtual void readPackets() noexcept = 0;

        /**
         * @brief Sends a packet through the server.
         * @param pkt The packet to be sent.
         * @return True if the packet was sent successfully, false otherwise.
         */
        virtual bool sendPacket(const IPacket &pkt) noexcept = 0;

        /**
         * @brief Checks if the stored IP address is valid.
         * @return True if the stored IP address is valid, false otherwise.
         */
        virtual bool isStoredIpCorrect() const noexcept = 0;

        /**
         * @brief Checks if the stored port number is valid.
         * @return True if the stored port number is valid, false otherwise.
         */
        virtual bool isStoredPortCorrect() const noexcept = 0;

        /**
         * @brief Pops a received packet from the server's packet queue.
         * @param pkt Reference to a Net::IPacket where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the queue was empty.
         */
        virtual bool popPacket(std::shared_ptr<IPacket> &pkt) noexcept = 0;
    };
} // namespace Net::Server
