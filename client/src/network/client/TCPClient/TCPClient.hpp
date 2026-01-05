/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** TCPClient
*/

#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <vector>
#include "ANetClient.hpp"
#include "IPacket.hpp"
#include "NetWrapper.hpp"
#include "RingBuffer/RingBuffer.hpp"
#include "TCPPacket.hpp"
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <errno.h>
    #include <unistd.h>
#endif

namespace Network
{
    /**
     * @class TCPClientError
     * @brief Exception class for TCPClient-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class TCPClientError : public std::exception {
      public:
        /**
         * @brief Constructor for TCPClientError.
         * @param message The error message to be associated with the exception.
         */
        explicit TCPClientError(const std::string &message) : _message("\n\t" + message)
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
     * @class TCPClient
     * @brief Network client for handling TCP communication with the server.
     * Inherits from ANetClient and implements TCP-specific functionality.
     */
    class TCPClient final : public ANetClient {
      public:
        /**
         * @brief Constructor for TCPClient.
         * @param net Shared pointer to a NetWrapper for network operations.
         * @throws TCPClientError if NetWrapper is null.
         */
        explicit TCPClient(std::shared_ptr<Net::NetWrapper> net = std::make_shared<Net::NetWrapper>("NetPluginLib"));

        /**
         * @brief Destructor for TCPClient.
         * Cleans up resources and closes the network connection.
         */
        ~TCPClient() override;

        /**
         * @brief Starts the TCP client.
         * @throws TCPClientError if an error occurs while starting the client.
         */
        void start() override;

        /**
         * @brief Closes the TCP client connection.
         */
        void close() override;

        /**
         * @brief Sets the socket to non-blocking or blocking mode.
         * @param nonBlocking True to set non-blocking mode, false for blocking mode.
         */
        void setNonBlocking(bool nonBlocking) override;

        /**
         * @brief Receives packets from the server.
         */
        void receivePackets() override;

        /**
         * @brief Sends a packet to the server.
         * @param pkt The packet to be sent.
         * @return True if the packet was sent successfully, false otherwise.
         */
        [[nodiscard]] bool sendPacket(const Net::IPacket &pkt) override;

        /**
         * @brief Pops a received packet from the internal queue.
         * @param pkt Reference to a shared pointer where the popped packet will be stored.
         * @return True if a packet was successfully popped, false if the queue was empty.
         */
        [[nodiscard]] bool popPacket(std::shared_ptr<Net::IPacket> &pkt) override;

        /**
         * @brief Retrieves a templated packet for serialization.
         * @return A shared pointer to the templated IPacket.
         */
        [[nodiscard]] std::shared_ptr<Net::IPacket> getTemplatedPacket() const noexcept override;

      private:
        /**
         * @brief Flushes pending writes to the TCP socket.
         * @note This method is called internally to ensure all queued data is sent.
         */
        void flushWrites() noexcept;

        /**
         * @brief Parses received data into complete frames.
         * @note This method processes the internal receive buffer to extract complete packets.
         */
        void parseFrames() noexcept;

        /**
         * @brief Resets the internal state of the TCP client.
         * @note This method clears buffers and resets positions.
         */
        void resetState() noexcept;

        /**
         * @brief Enqueues a frame into the transmit buffer in a thread-safe manner.
         * @param beSize The big-endian size of the packet to be enqueued.
         * @param pkt The packet to be enqueued.
         * @param size The size of the packet to be enqueued.
         * @return True if the frame was successfully enqueued, false otherwise.
         */
        [[nodiscard]] bool enqueueFrameLocked(std::uint32_t beSize, const Net::IPacket &pkt, std::uint32_t size);

        /**
         * @brief Attempts to enqueue a frame into the transmit buffer.
         * @param beSize The big-endian size of the packet to be enqueued.
         * @param pkt The packet to be enqueued.
         * @param size The size of the packet to be enqueued.
         * @return True if the frame was successfully enqueued, false otherwise.
         */
        [[nodiscard]] bool tryEnqueueFrame(std::uint32_t beSize, const Net::IPacket &pkt, std::uint32_t size);

        /**
         * @brief Checks if a packet is sendable and retrieves its size.
         * @param pkt The packet to be checked.
         * @param outSize Reference to a variable where the size of the packet will be stored.
         * @return True if the packet is sendable, false otherwise.
         */
        [[nodiscard]] bool isSendable(const Net::IPacket &pkt, std::uint32_t &outSize) const noexcept;

        std::shared_ptr<Net::NetWrapper> _netWrapper; ///> Network wrapper

        sockaddr_in _serverAddr{}; ///> Server address structure
        bool _nonBlocking = true;  ///> Non-blocking mode flag

        Buffer::RingBuffer<uint8_t> _rx; ///> Receive buffer

        std::mutex _txMutex;             ///> Mutex for synchronizing access to the transmit buffer
        Buffer::RingBuffer<uint8_t> _tx; ///> Receive buffer

        std::mutex _queueMutex;                           ///> Mutex for synchronizing access to the packet queue
        std::queue<std::shared_ptr<Net::IPacket>> _queue; ///> Queue of received packets

        static constexpr std::uint32_t MAX_FRAME = 64 * 1024; ///> Maximum frame size (64 KB)

        static constexpr int RX_CAPACITY = 256 * 1024; ///> Receive buffer capacity (256 KB)
        static constexpr int TX_CAPACITY = 256 * 1024; ///> Transmit buffer capacity (256 KB)
    };
} // namespace Network
