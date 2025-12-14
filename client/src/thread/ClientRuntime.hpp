/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "EntitiesFactory.hpp"
#include "NetClient.hpp"

namespace Thread
{

    /**
     * @brief The ClientRuntime class is responsible for managing the client's runtime operations,
     * including starting, stopping, and handling incoming packets.
     */
    class ClientRuntime {
      public:
        /**
         * @brief Constructor for ClientRuntime.
         * @param client Shared pointer to the network client.
         */
        explicit ClientRuntime(const std::shared_ptr<Network::NetClient> &client);

        /**
         * @brief Destructor for ClientRuntime.
         */
        ~ClientRuntime();

        /**
         * @brief Starts the client runtime operations.
         */
        void start();

        /**
         * @brief Stops the client runtime operations.
         */
        void stop();

        /**
         * @brief Waits for the client runtime threads to finish.
         */
        void wait();

      private:
        std::shared_ptr<Network::NetClient> _client;            ///> Network client
        std::shared_ptr<Ecs::EntitiesFactory> _entitiesFactory; ///> Entities factory

        std::thread _receiverThread; ///> Thread for receiving packets
        std::thread _updateThread;   ///> Thread for updating game state
        // std::thread _renderThread;    ///> Thread for rendering graphics
        // std::thread _inputThread;     ///> Thread for handling user input

        std::mutex _mutex;                 ///> Mutex for synchronizing access
        std::condition_variable _cv;       ///> Condition variable for signaling
        bool _stopRequested = false;       ///> Flag to indicate if a stop has been requested
        std::atomic<bool> _running{false}; ///> Atomic flag to indicate if the client is running

        void runReceiver() const; ///> Method for running the receiver thread
        void runUpdater() const;  ///> Method for running the updater thread
    };

} // namespace Thread
