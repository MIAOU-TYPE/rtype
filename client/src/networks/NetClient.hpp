/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#pragma once

#include <mutex>
#include <variant>
#include <vector>
#include "ANetClient.hpp"
#include "NetWrapper.hpp"
#include "RingBuffer.hpp"
#include "UDPPacket.hpp"

namespace Network
{

    /**
     * @class NetClientError
     * @brief Exception class for network client-related errors.
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
        std::string _message; ///> Error message
    };

    /**
     * @class NetClient
     * @brief Network client for handling UDP communication with the server.
     */
    class NetClient : public ANetClient {
      public:
        /**
         * @brief Constructor for NetClient.
         * Initializes the network wrapper, creates a UDP socket, and sets up the server address.
         * @throws NetClientError if socket creation fails.
         */
        NetClient();

        /**
         * @brief Destructor for NetClient.
         * Closes the network connection and cleans up resources.
         */
        ~NetClient() override;

        /**
         * @brief Receives packets from the server.
         */
        void receivePackets() override;

        /**
         * @brief Closes the network client connection.
         */
        void close() override;

        void start() override;

        bool sendPacket(const Net::IPacket &pkt) override;

        bool popPacket(std::shared_ptr<Net::IPacket> &pkt) override;

      private:
        Net::NetWrapper _netWrapper; ///> Network wrapper
        std::mutex _packetDataMutex; ///> Mutex for synchronizing access to packet data
        Buffer::RingBuffer<std::shared_ptr<Net::IPacket>> _ringBuffer; ///> Ring buffer to store received packets
    };

} // namespace Network