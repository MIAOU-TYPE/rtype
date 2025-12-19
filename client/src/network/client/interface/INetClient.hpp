/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** INetClient
*/

#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
#include "IPacket.hpp"

/**
 * @namespace NetClient
 * @brief Contains all NetClient-related classes and interfaces.
 */
namespace Network
{
    /**
     * @class NetClientError
     * @brief Exception class for NetClient-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class NetClientError : public std::exception {
      public:
        /**
         * @brief Constructor for NetClientError.
         * @param message The error message to be associated with the exception.
         */
        explicit NetClientError(const std::string &message) : _message("\n\t" + message)
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
     * @interface INetClient
     * @brief Interface for a NetClient.
     * Defines the essential methods for configuring, starting, closing,
     * and checking the status of a NetClient.
     * @note This interface should be implemented by any NetClient class.
     */
    class INetClient {
      public:
        /**
         * @brief Virtual destructor for INetClient interface.
         * Ensures proper cleanup of derived classes.
         * @note Always declare destructors in interfaces as virtual.
         */
        virtual ~INetClient() = default;

        /**
         * @brief Configures the NetClient with the specified IP address and port.
         * @param ip The IP address to bind the NetClient to.
         * @param port The port number to listen on.
         */
        virtual void configure(const std::string &ip, int32_t port) = 0;

        /**
         * @brief Starts the NetClient.
         * @note This method should initiate the NetClient's main loop or listening process.
         */
        virtual void start() = 0;

        /**
         * @brief closes the NetClient.
         * @note This method should gracefully shut down the NetClient and release resources.
         */
        virtual void close() = 0;

        /**
         * @brief Sets the NetClient's socket to non-blocking or blocking mode.
         * @param nonBlocking True to set the socket to non-blocking mode, false for blocking mode.
         */
        virtual void setNonBlocking(bool nonBlocking) = 0;

        /**
         * @brief Checks if the NetClient is currently running.
         * @return True if the NetClient is running, false otherwise.
         */
        virtual bool isRunning() const noexcept = 0;

        /**
         * @brief Sets the running status of the NetClient.
         * @param running True to set the NetClient as running, false otherwise.
         */
        virtual void setRunning(bool running) noexcept = 0;

        /**
         * @brief reads packets from the NetClient.
         */
        virtual void receivePackets() = 0;

        /**
         * @brief Sends a packet through the NetClient.
         * @param pkt The packet to be sent.
         * @return True if the packet was sent successfully, false otherwise.
         */
        virtual bool sendPacket(const Net::IPacket &pkt) = 0;

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
         * @brief Pops a received packet from the NetClient's packet queue.
         * @param pkt Reference to a Net::IPacket where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the queue was empty.
         */
        virtual bool popPacket(std::shared_ptr<Net::IPacket> &pkt) = 0;

        /**
         * @brief Retrieves a templated packet for serialization.
         * @return A shared pointer to the templated IPacket.
         */
        virtual std::shared_ptr<Net::IPacket> getTemplatedPacket() const noexcept = 0;
    };
} // namespace Network
