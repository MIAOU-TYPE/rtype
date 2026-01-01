/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPServer
*/

#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

#include <cstring>
#include <iostream>
#include <memory>
#include <ranges>
#include "AServer.hpp"
#include "Endian.hpp"
#include "NetWrapper.hpp"
#include "RingBuffer/RingBuffer.hpp"
#include "TCPPacket.hpp"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <errno.h>
    #include <netinet/in.h>
#endif

namespace Net::Server
{
    /**
     * @class TCPServer
     * @brief A TCP server implementation of the AServer interface.
     * This class handles TCP connections, packet reading, and sending.
     */
    class TCPServer final : public AServer {
      public:
        /**
         * @brief Construct a new TCPServer object with a custom NetWrapper.
         * @param wrapper The NetWrapper instance to use for network operations.
         */
        explicit TCPServer(const std::shared_ptr<NetWrapper> &wrapper = std::make_shared<NetWrapper>("NetPluginLib"));

        /**
         * @brief Destructor for TCPServer.
         */
        ~TCPServer() override;

        /**
         * @brief Start the TCP server.
         * @throws ServerError if the server fails to start.
         */
        void start() override;

        /**
         * @brief Stop the TCP server.
         */
        void stop() override;

        /**
         * @brief Set the non-blocking mode for the server.
         * @param nonBlocking True to set non-blocking mode, false for blocking mode.
         */
        void setNonBlocking(bool nonBlocking) override;

        /**
         * @brief Read incoming packets from clients.
         * @note This function should be called periodically to process incoming data.
         * @throws ServerError if reading packets fails.
         */
        void readPackets() override;

        /**
         * @brief Send a packet to a client.
         * @param pkt The packet to send.
         * @return true if the packet was sent successfully, false otherwise.
         */
        bool sendPacket(const IPacket &pkt) override;

        /**
         * @brief Pop a received packet from the internal queue.
         * @param pkt Reference to a shared pointer where the popped packet will be stored.
         * @return true if a packet was popped, false if the queue is empty.
         */
        bool popPacket(std::shared_ptr<IPacket> &pkt) override;

      private:
        /*
         * @brief Snapshot the current client sockets.
         * @return A vector of current client socket handles.
         */
        std::vector<socketHandle> snapshotClientSockets() const;

        /**
         * @brief Read data from a single client.
         * @param clientFd Socket handle of the client to read from.
         * @param tmp Temporary buffer for reading data.
         */
        void readOneClient(socketHandle clientFd, std::array<uint8_t, 4096> &tmp);

        /**
         * @brief Handle received bytes from a client.
         * @param clientFd Socket handle of the client.
         * @param data Pointer to the received data.
         * @param len Length of the received data.
         * @return true if the data was processed successfully, false otherwise.
         */
        bool onBytesReceived(socketHandle clientFd, const uint8_t *data, size_t len);

        /**
         * @brief Accept incoming client connections in a loop.
         * This function handles new client connections and adds them to the client list.
         * @note This function is called internally by readPackets().
         */
        void acceptLoop();

        /**
         * @brief Read data from connected clients.
         */
        void readClients();

        /**
         * @brief Flush pending writes to a client.
         * @param clientFd Socket handle of the client to flush writes to.
         */
        void flushWrites(socketHandle clientFd);

        /**
         * @brief Drop a client connection.
         * @param clientFd Socket handle of the client to drop.
         */
        void dropClient(socketHandle clientFd);

        std::shared_ptr<NetWrapper> _netWrapper; ///> Network wrapper for socket operations
        std::shared_ptr<IPacket> _packet;        ///> Packet prototype for creating packets

        bool _nonBlocking = true; ///> Non-blocking mode flag

        socketHandle _listenFd = kInvalidSocket; ///> Listening socket handle

        /**
         * @struct ClientState
         * @brief Represents the state of a connected client.
         */
        struct ClientState {
            explicit ClientState(const sockaddr_in addr) : addr(addr), rx(MAXSIZE), tx(MAXSIZE)
            {
            }

            sockaddr_in addr{};             ///> Client address
            Buffer::RingBuffer<uint8_t> rx; ///> Receive buffer
            Buffer::RingBuffer<uint8_t> tx; ///> Transmit buffer
        };

        mutable std::mutex _mutex; ///> Mutex for protecting client maps
        std::unordered_map<socketHandle, std::unique_ptr<ClientState>>
            _clients; ///> Map of client socket handles to their states
        std::unordered_map<AddressKey, socketHandle, AddressKeyHash>
            _endpointToFd; ///> Map of endpoint keys to socket handles

        mutable std::mutex _queueMutex;              ///> Mutex for protecting the packet queue
        std::queue<std::shared_ptr<IPacket>> _queue; ///> Queue of received packets

        static constexpr uint32_t MAXSIZE = 64 * 1024; ///> Maximum allowed frame size
    };
} // namespace Net::Server
