/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "NetClient.hpp"

namespace Thread {

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
        std::shared_ptr<Network::NetClient> _client; ///> Network client

        std::thread _receiverThread;  ///> Thread for receiving packets
        // std::thread _updateThread;    ///> Thread for updating game state
        // std::thread _renderThread;    ///> Thread for rendering graphics
        // std::thread _inputThread;     ///> Thread for handling user input

        std::mutex _mutex;                 ///> Mutex for synchronizing access
        std::condition_variable _cv;       ///> Condition variable for signaling
        bool _stopRequested = false;       ///> Flag to indicate if a stop has been requested
        std::atomic<bool> _running{false}; ///> Atomic flag to indicate if the server is running

        void runReceiver() const;          ///> Method for running the receiver thread
};

} // namespace Thread
