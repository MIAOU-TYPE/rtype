/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ANetworkClient
*/

#pragma once
#include "INetworkClient.hpp"
#include "SocketConfig.hpp"
#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <queue>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

/**
 * @namespace Network
 * @brief Namespace for network-related classes and functions.
 */
namespace Network
{
    /**
     * @class ANetworkClient
     * @brief Abstract base class for network clients.
     * @details This class provides common functionality for network client management,
     * including connection state, socket configuration, and packet queue management.
     * Derived classes must implement specific protocol behaviors (UDP, TCP, etc.).
     * @note This class inherits from INetworkClient.
     */
    class ANetworkClient : public INetworkClient {
      public:
        /**
         * @brief Virtual destructor for ANetworkClient.
         */
        virtual ~ANetworkClient() = default;

        /**
         * @brief Connect to the game server
         * @param ip Server IP address
         * @param port Server port number
         */
        void connectToServer(const std::string &ip, uint16_t port) override = 0;

        /**
         * @brief Disconnect from the game server
         */
        void disconnectFromServer() override = 0;

        /**
         * @brief Send a packet to the server
         * @param data Serialized packet data
         */
        void sendPacket(const std::vector<uint8_t> &data) override = 0;

        /**
         * @brief Poll for incoming packets with metadata from the server
         * @return std::vector<std::shared_ptr<IClientPacket>> List of received packets with metadata
         */
        std::vector<std::shared_ptr<IClientPacket>> pollPackets() override;

        /**
         * @brief Check if the client is connected to the server
         * @return true if connected, false otherwise
         */
        bool isConnected() const override;

      protected:
        /**
         * @brief Constructor for ANetworkClient.
         */
        ANetworkClient();

        /**
         * @brief Sets up the socket with specified configuration
         * @param config Socket configuration parameters
         * @throws Client::Exception::SocketException if socket creation fails
         */
        void setupSocket(const SocketConfig &config);

        /**
         * @brief Applies socket options
         * @param options Socket options to apply
         * @throws Client::Exception::SocketException if setting options fails
         */
        void applySocketOptions(const SocketOptions &options);

        /**
         * @brief Validates and stores the server address
         * @param ip Server IP address
         * @param port Server port number
         * @throws Client::Exception::ConnectionException if IP/port is invalid
         */
        void configureServerAddress(const std::string &ip, uint16_t port);

        /**
         * @brief Sets the socket to non-blocking or blocking mode
         * @param nonBlocking True to set non-blocking mode, false for blocking mode
         * @throws Client::Exception::SocketException if fcntl fails
         */
        void setNonBlocking(bool nonBlocking = true);

        /**
         * @brief Checks if the stored IP address is valid
         * @return true if the IP is valid, false otherwise
         */
        bool isStoredIpCorrect() const noexcept;

        /**
         * @brief Checks if the stored port is valid
         * @return true if the port is valid, false otherwise
         */
        bool isStoredPortCorrect() const noexcept;

        int _socketFd;                                                ///< Socket file descriptor
        sockaddr_in _serverAddr;                                      ///< Server address structure
        std::string _serverIp;                                        ///< Server IP address (for validation)
        uint16_t _serverPort;                                         ///< Server port (for validation)
        std::thread _receiverThread;                                  ///< Thread for receiving packets
        std::atomic<bool> _running;                                   ///< Flag to control receiver thread
        std::mutex _receiveMutex;                                     ///< Mutex for thread-safe packet access
        std::queue<std::shared_ptr<IClientPacket>> _receivedPackets;  ///< Queue of received packets with metadata
        SocketConfig _socketConfig;                                   ///< Socket configuration
    };
} // namespace Network
