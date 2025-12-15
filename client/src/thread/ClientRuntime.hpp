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

#include "ClientPacketFactory.hpp"
#include "DisplayInit.hpp"
#include "EventInit.hpp"
#include "NetClient.hpp"
#include "PacketRouter.hpp"
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
    class ClientRuntimeError : public std::exception {
      public:
        explicit ClientRuntimeError(const std::string &message) : _message("\n\t" + message)
        {
        }

        const char *what() const noexcept
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    class ClientRuntime {
      public:
        /**
         * @brief Constructor for ClientRuntime.
         * @param client Shared pointer to the network client.
         */
        explicit ClientRuntime(const std::shared_ptr<Network::INetClient> &client);

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
        std::shared_ptr<Graphics::SFMLRenderer> _renderer; ///> Shared renderer

        std::shared_ptr<Network::INetClient> _client;                 ///> Network client
        std::shared_ptr<Ecs::PacketRouter> _packetRouter;             ///> Entities factory
        std::shared_ptr<Network::ClientPacketFactory> _packetFactory; ///> Packet factory
        std::shared_ptr<Display::DisplayInit> _display;               ///> Display manager
        std::shared_ptr<Events::EventInit> _event;                    ///> Event manager

        std::thread _receiverThread; ///> Thread for receiving packets
        std::thread _updateThread;   ///> Thread for updating game state
        std::thread _displayThread;  ///> Thread for displaying graphics
        std::thread _eventThread;    ///> Thread for handling events

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
