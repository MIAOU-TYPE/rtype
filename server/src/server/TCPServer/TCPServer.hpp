/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPServer
*/

#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

#include "AServer.hpp"
#include "Endian.hpp"
#include "NetWrapper.hpp"
#include "TCPPacket.hpp"

#include <cstring>
#include <iostream>
#include <ranges>

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
         * @brief Construct a new TCPServer object.
         */
        TCPServer();

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
         * @brief Drop a client connection.
         * @param clientFd Socket handle of the client to drop.
         * @param clientAddr Address of the client to drop.
         */
        void dropClient(socketHandle clientFd, const sockaddr_in &clientAddr);

        /**
         * @brief Send all data to a socket.
         * @param clientFd Socket handle to send data to.
         * @param data Pointer to the data to send.
         * @param len Length of the data to send.
         * @return true if all data was sent successfully, false otherwise.
         */
        bool sendAll(socketHandle clientFd, const uint8_t *data, size_t len) const;

        NetWrapper _netWrapper;           ///> Network wrapper for socket operations
        std::shared_ptr<IPacket> _packet; ///> Packet prototype for creating packets

        bool _nonBlocking = true; ///> Non-blocking mode flag

        socketHandle _listenFd = kInvalidSocket; ///> Listening socket handle

        /**
         * @struct ClientState
         * @brief Represents the state of a connected client.
         */
        struct ClientState {
            sockaddr_in addr{};      ///> Client address
            std::vector<uint8_t> rx; ///> Receive buffer
        };

        mutable std::mutex _mutex;                              ///> Mutex for protecting client maps
        std::unordered_map<socketHandle, ClientState> _clients; ///> Map of client socket handles to their states
        std::unordered_map<AddressKey, socketHandle, AddressKeyHash>
            _endpointToFd; ///> Map of endpoint keys to socket handles

        mutable std::mutex _queueMutex;              ///> Mutex for protecting the packet queue
        std::queue<std::shared_ptr<IPacket>> _queue; ///> Queue of received packets

        static constexpr uint32_t MAXSIZE = 64 * 1024; ///> Maximum allowed frame size
    };
} // namespace Net::Server
