/*
** EPITECH PROJECT, 2025
** RType
** File description:
** ServerRuntime.hpp
*/

#pragma once
#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include "GameServer.hpp"
#include "IServer.hpp"
#include "RoomManager.hpp"
#include "SessionManager.hpp"
#include "SnapshotSystem.hpp"
#include "TCPPacket.hpp"
#include "TCPPacketFactory.hpp"
#include "TCPPacketRouter.hpp"
#include "UDPPacket.hpp"
#include "UDPPacketFactory.hpp"
#include "UDPPacketRouter.hpp"
#include <condition_variable>

namespace Net::Thread
{
    /**
     * @class ThreadError
     * @brief Exception class for thread-related errors.
     */
    class ThreadError : public std::exception {
      public:
        /**
         * @brief Constructs a new ThreadError object with the specified message.
         * @param message The error message.
         */
        explicit ThreadError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///> The error message.
    };

    /**
     * @brief The ServerRuntime class is responsible for managing the server's runtime operations,
     * including starting, stopping, and handling incoming packets.
     */
    class ServerRuntime {
      public:
        /**
         * @brief Construct a new Server Runtime object
         * @param UdpServer A shared pointer to the UDP server instance
         * @param TcpServer A shared pointer to the TCP server instance
         */
        explicit ServerRuntime(
            const std::shared_ptr<Server::IServer> &UdpServer, const std::shared_ptr<Server::IServer> &TcpServer);

        /**
         * @brief Destroy the Server Runtime object
         */
        ~ServerRuntime();

        /**
         * @brief Wait for the server to stop
         */
        void wait();

        /**
         * @brief Start the server runtime
         * Launches threads for receiving and processing packets
         */
        void start();

        /**
         * @brief Stop the server runtime
         * Safely stops the server and joins threads
         */
        void stop();

      private:
        /**
         * @brief Thread function to handle receiving packets
         */
        void runReceiver() const;

        /**
         * @brief Thread function to handle processing packets
         */
        void runProcessor() const;

        /**
         * @brief Thread function to handle taking snapshots of the game state
         */
        void runSnapshot() const;

        /**
         * @brief Thread function to handle TCP connections and packets
         */
        void runTcp() const;

        std::shared_ptr<Server::IServer> _udpServer;       ///> The server instance
        std::shared_ptr<UDPPacketRouter> _udpPacketRouter; ///> Routes incoming packets to appropriate handlers
        std::shared_ptr<Factory::UDPPacketFactory> _udpPacketFactory; ///> Builds outgoing packets.

        std::shared_ptr<Server::IServer> _tcpServer;       ///> The TCP server instance
        std::shared_ptr<TCPPacketRouter> _tcpPacketRouter; ///> Routes incoming TCP packets to appropriate handlers
        std::shared_ptr<Factory::TCPPacketFactory> _tcpPacketFactory; ///> Builds outgoing TCP packets.

        std::shared_ptr<Server::ISessionManager> _sessionManager; ///> Manages client sessions
        std::shared_ptr<Engine::RoomManager> _roomManager;        ///> Manages game rooms

        std::thread _receiverThread;  ///> Thread for receiving packets
        std::thread _processorThread; ///> Thread for processing packets
        std::thread _snapshotThread;  ///> Thread for handling snapshots
        std::thread _tcpThread;       ///> Thread for handling TCP packets

        std::mutex _mutex;                       ///> Mutex for synchronizing access
        std::condition_variable _cv;             ///> Condition variable for signaling
        std::atomic<bool> _stopRequested{false}; ///> Flag to indicate if a stop has been requested
        std::atomic<bool> _running{false};       ///> Atomic flag to indicate if the server is running
    };
} // namespace Net::Thread