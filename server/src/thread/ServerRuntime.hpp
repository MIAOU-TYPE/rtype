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
#include "PacketFactory.hpp"
#include "PacketRouter.hpp"
#include "SessionManager.hpp"
#include "SnapshotSystem.hpp"
#include "UDPPacket.hpp"
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
         * @param server A shared pointer to the server instance
         */
        explicit ServerRuntime(const std::shared_ptr<Server::IServer> &server);

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
         * @brief Thread function to handle updating game state
         */
        void runUpdate() const;

        /**
         * @brief Thread function to handle taking snapshots of the game state
         */
        void runSnapshot() const;

        std::shared_ptr<Server::IServer> _server;                 ///> The server instance
        std::shared_ptr<Server::ISessionManager> _sessionManager; ///> Manages client sessions
        std::shared_ptr<Game::GameServer> _gameServer;            ///> The game server logic
        std::shared_ptr<PacketRouter> _packetRouter;              ///> Routes incoming packets to appropriate handlers
        std::shared_ptr<Factory::PacketFactory> _packetFactory;   ///> Builds outgoing packets.

        std::thread _receiverThread;  ///> Thread for receiving packets
        std::thread _processorThread; ///> Thread for processing packets
        std::thread _updateThread;    ///> Thread for updating game state
        std::thread _snapshotThread;  ///> Thread for handling snapshots

        std::mutex _mutex;                       ///> Mutex for synchronizing access
        std::condition_variable _cv;             ///> Condition variable for signaling
        std::atomic<bool> _stopRequested{false}; ///> Flag to indicate if a stop has been requested
        std::atomic<bool> _running{false};       ///> Atomic flag to indicate if the server is running
    };
} // namespace Net::Thread