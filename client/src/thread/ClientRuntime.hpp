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

#include "DisplayInit.hpp"
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
        ~ClientRuntime();

        void start();
        void stop();
        void wait();

            private:
        std::shared_ptr<Network::NetClient> _client;    ///> Network client
        std::shared_ptr<Display::DisplayInit> _display; ///> Display manager

        std::thread _receiverThread; ///> Thread for receiving packets
        // std::thread _updateThread;    ///> Thread for updating game state
        std::thread _displayThread; ///> Thread for displaying graphics
        // std::thread _inputThread;     ///> Thread for handling user input

        std::mutex _mutex;                 ///> Mutex for synchronizing access
        std::condition_variable _cv;       ///> Condition variable for signaling
        bool _stopRequested = false;       ///> Flag to indicate if a stop has been requested
        std::atomic<bool> _running{false}; ///> Atomic flag to indicate if the server is running

        void runReceiver() const; ///> Method for running the receiver thread
        void runDisplay() const;  ///> Method for running the display thread
    };

} // namespace Thread
