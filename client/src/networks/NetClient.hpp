/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#pragma once

#include "DamageData.hpp"
#include "DefaultData.hpp"
#include "Endian.hpp"
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
#include "InputData.hpp"
#include "NetWrapper.hpp"
#include "TypesData.hpp"
#include "UDPPacket.hpp"

namespace Game
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
    class NetClient {
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
        ~NetClient();

        /**
         * @brief Sends a connect packet to the server.
         */
        void sendConnectPacket();

        /**
         * @brief Sends an input packet to the server.
         * @param dx The change in x position.
         * @param dy The change in y position.
         * @param shooting Whether the player is shooting.
         */
        void sendInputPacket(int8_t dx, int8_t dy, bool shooting);

        /**
         * @brief Receives packets from the server.
         */
        void receivePackets();

        /**
         * @brief Handles a received UDP packet.
         * @param packet The received UDP packet.
         */
        void handlePacket(const Net::UDPPacket &packet);

        /**
         * @brief Sends a disconnect packet to the server.
         */
        void sendDisconnectPacket();

        /**
         * @brief Sends a ping packet to the server.
         */
        void sendPingPacket();

        /**
         * @brief Updates the ping timer and sends ping packets as needed.
         * @param deltaTime The time elapsed since the last update.
         */
        void updatePing(float deltaTime);

        /**
         * @brief Closes the network client connection.
         */
        void close();

      private:
        std::unique_ptr<Net::NetWrapper> _netWrapper = nullptr; ///> Network wrapper
        socketHandle _socket = kInvalidSocket;                  ///> UDP socket
        sockaddr_in _serverAddr = {};                           ///> Server address
        bool _isConnected = false;                              ///> Connection status
        uint32_t _playerEntityId = 0;                           ///> Player entity ID
        float _pingTimer = 0.0f;                                ///> Ping timer
        static constexpr float PING_INTERVAL = 5.0f;            ///> Ping interval in seconds
    };

} // namespace Game