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
#include "IServer.hpp"
#include "PacketRouter.hpp"
#include "SessionManager.hpp"
#include "game/gameServer/GameServer.hpp"
#include <condition_variable>

namespace Net::Thread
/**
 * @brief The ServerRuntime class is responsible for managing the server's runtime operations,
 * including starting, stopping, and handling incoming packets.
 */
{
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

        std::shared_ptr<Net::Server::IServer> _server;           ///> The server instance
        std::shared_ptr<Server::SessionManager> _sessionManager; ///> Manages client sessions
        std::shared_ptr<Game::GameServer> _gameServer;           ///> The game server logic
        std::shared_ptr<PacketRouter> _packetRouter;             ///> Routes incoming packets to appropriate handlers

        std::thread _receiverThread;  ///> Thread for receiving packets
        std::thread _processorThread; ///> Thread for processing packets

        std::mutex _mutex;           ///> Mutex for synchronizing access
        std::condition_variable _cv; ///> Condition variable for signaling
        bool _stopRequested = false; ///> Flag to indicate if a stop has been requested
    };
}