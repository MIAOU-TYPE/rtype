/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "CommandBuffer.hpp"
#include "InputData.hpp"
#include "NetWrapper.hpp"
#include "TypesData.hpp"
#include "UDPPacket.hpp"

namespace Game
{
    /**
     * @struct NetworkCommand
     * @brief Represents a received network packet encapsulated as a command.
     */
    struct NetworkCommand {
        std::vector<uint8_t> data; ///> Packet data buffer
        sockaddr_in senderAddr;    ///> Sender address

        /**
         * @brief Default constructor.
         */
        NetworkCommand() : senderAddr{}
        {
        }

        /**
         * @brief Constructs a NetworkCommand from a UDPPacket.
         * @param packet The received UDP packet.
         */
        explicit NetworkCommand(const Net::UDPPacket &packet)
            : data(packet.buffer(), packet.buffer() + packet.size()), senderAddr(*packet.address())
        {
        }
    };

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
         * Initializes the network wrapper, creates a UDP socket, sets up the server address,
         * and starts the reception thread.
         * @param commandBuffer Shared pointer to the command buffer for received packets.
         * @throws NetClientError if socket creation fails.
         */
        explicit NetClient(std::shared_ptr<Command::CommandBuffer<NetworkCommand>> commandBuffer);

        /**
         * @brief Destructor for NetClient.
         * Stops the reception thread, closes the network connection and cleans up resources.
         */
        ~NetClient();

        /**
         * @brief Construct a new Net Client object (non-copyable)
         */
        NetClient(const NetClient &) = delete;

        /**
         * @brief Copy assignment operator (non-copyable)
         */
        NetClient &operator=(const NetClient &) = delete;

        /**
         * @brief Construct a new Net Client object (Movable)
         */
        NetClient(NetClient &&) = delete;

        /**
         * @brief Move assignment operator (Movable)
         */
        NetClient &operator=(NetClient &&) = delete;

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
         * @brief Gets the current measured latency.
         * @return The latency in seconds.
         */
        float getLatency() const;

        /**
         * @brief Checks if the client is connected to the server.
         * @return True if connected, false otherwise.
         */
        bool isConnected() const;

        /**
         * @brief Closes the network client connection and stops the reception thread.
         */
        void close();

        /**
         * @brief Starts the network reception thread.
         */
        void startReceptionThread();

        /**
         * @brief Stops the network reception thread.
         */
        void stopReceptionThread();

        /**
         * @brief Checks if the client is connected to the server.
         * @return true if connected, false otherwise.
         */
        bool isConnected() const noexcept
        {
            return _isConnected;
        }

        /**
         * @brief Gets the player entity ID assigned by the server.
         * @return The player entity ID.
         */
        uint32_t getPlayerEntityId() const noexcept
        {
            return _playerEntityId;
        }

        /**
         * @brief Sets the connection status.
         * @param connected The new connection status.
         */
        void setConnected(bool connected) noexcept
        {
            _isConnected = connected;
        }

        /**
         * @brief Sets the player entity ID.
         * @param entityId The player entity ID assigned by the server.
         */
        void setPlayerEntityId(uint32_t entityId) noexcept
        {
            _playerEntityId = entityId;
        }

      private:
        /**
         * @brief Reception thread main loop.
         * Blocks on recvfrom() and pushes received packets into the CommandBuffer.
         */
        void receptionLoop();

        std::unique_ptr<Net::NetWrapper> _netWrapper = nullptr; ///> Network wrapper
        socketHandle _socket = kInvalidSocket;                  ///> UDP socket
        sockaddr_in _serverAddr = {};                           ///> Server address
        bool _isConnected = false;                              ///> Connection status
        uint32_t _playerEntityId = 0;                           ///> Player entity ID
        float _pingTimer = 0.0f;                                ///> Ping timer
        float _lastPingTime = 0.0f;                             ///> Timestamp when last ping was sent
        float _latency = 0.0f;                                  ///> Current latency in seconds
        bool _waitingForPong = false;                           ///> Flag to track if waiting for pong response
        uint8_t _missedPongCount = 0;                           ///> Counter for consecutive missed pongs
        static constexpr float PING_INTERVAL = 5.0f;            ///> Ping interval in seconds
        static constexpr float PONG_TIMEOUT = 10.0f;   ///> Timeout to consider a pong as missed (2x ping interval)
        static constexpr uint8_t MAX_MISSED_PONGS = 3; ///> Maximum consecutive missed pongs before disconnect

        // Reception thread members
        std::shared_ptr<Command::CommandBuffer<NetworkCommand>> _commandBuffer; ///> Shared command buffer
        std::thread _rxThread;                                                   ///> Reception thread
        std::atomic<bool> _running{false};                                       ///> Thread running flag
    };

} // namespace Game