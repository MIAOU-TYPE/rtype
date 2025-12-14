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
#include "../events/EventInit.hpp"
#include "DisplayInit.hpp"
#include "NetClient.hpp"
#include "SFMLRenderer.hpp"

/**
 * @namespace Thread
 * @brief Contains all threading-related classes for the client.
 */
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
         * Cleans up resources and stops all threads.
         */
        ~ClientRuntime();

        void start();
        void stop();
        void wait();

      private:
        std::shared_ptr<Graphics::SFMLRenderer> _renderer; ///> Shared renderer

        std::shared_ptr<Network::NetClient> _client;               ///> Network client
        std::shared_ptr<Ecs::EntitiesFactory> _entitiesFactory; ///> Entities factory
        std::shared_ptr<Display::DisplayInit> _display; ///> Display manager
        std::shared_ptr<Events::EventInit> _event;      ///> Event manager

        std::thread _receiverThread; ///> Thread for receiving packets
        std::thread _updateThread;   ///> Thread for updating game state
        std::thread _displayThread; ///> Thread for displaying graphics
        std::thread _eventThread;   ///> Thread for handling events

        std::mutex _mutex;                 ///> Mutex for synchronizing access
        std::condition_variable _cv;       ///> Condition variable for signaling
        bool _stopRequested = false;       ///> Flag to indicate if a stop has been requested
        std::atomic<bool> _running{false}; ///> Atomic flag to indicate if the client is running

        void runReceiver() const; ///> Method for running the receiver thread
        void runUpdater() const;  ///> Method for running the updater thread
        void runDisplay();        ///> Method for running the display thread
        void runEvent();          ///> Method for running the event thread
    };

} // namespace Thread
