/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientRuntime
*/

#pragma once

#include <atomic>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "AssetLoader.hpp"
#include "ClientController.hpp"
#include "ClientPacketFactory.hpp"
#include "ClientWorld.hpp"
#include "EventRegistry.hpp"
#include "IGraphics.hpp"
#include "INetClient.hpp"
#include "PacketRouter.hpp"
#include "WorldCommandBuffer.hpp"

using steadyClock = std::chrono::steady_clock;

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
        /**
         * @brief Constructor for ClientRuntimeError.
         * @param message The error message.
         */
        explicit ClientRuntimeError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Override of the what() method from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
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
         * @param graphics Shared pointer to the graphics interface.
         * @param client Shared pointer to the network client interface.
         */
        explicit ClientRuntime(
            const std::shared_ptr<Graphics::IGraphics> &graphics, const std::shared_ptr<Network::INetClient> &client);

        /**
         * @brief Destructor for ClientRuntime.
         * Cleans up resources and stops the client runtime.
         */
        ~ClientRuntime();

        /**
         * @brief Starts the client runtime, including network communication and game state updates.
         * @throws ClientRuntimeError if the client fails to start.
         * @details This method initializes the network client and starts the receiver and updater threads.
         */
        void start();

        /**
         * @brief Stops the client runtime gracefully.
         * @details This method signals the receiver and updater threads to stop and waits for them to finish.
         */
        void stop();

        /**
         * @brief Waits for the client runtime to stop.
         * @details This method blocks until the client runtime has been stopped.
         */
        void wait();

        /**
         * @brief Retrieves the event bus used for handling events.
         * @return Shared pointer to the event bus.
         * @details The event bus is used for communication between different components of the client.
         */
        [[nodiscard]] std::shared_ptr<Core::EventBus> getEventBus() const noexcept;

        /**
         * @brief Runs the display loop for rendering graphics.
         * @details This method handles the rendering of graphics and user input.
         * It should be called from the main thread.
         */
        void runDisplay();

      private:
        std::shared_ptr<Core::EventBus> _eventBus = nullptr;           ///> Event bus for handling events
        std::unique_ptr<Core::EventRegistry> _eventRegistry = nullptr; ///> Event registry for managing event handlers

        std::shared_ptr<Network::INetClient> _client = nullptr; ///> Network client interface

        std::shared_ptr<Graphics::IGraphics> _graphics = nullptr; ///> Graphics interface
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr; ///> Renderer for graphics
        std::unique_ptr<Engine::ClientWorld> _world = nullptr;    ///> Client world for managing game state

        std::shared_ptr<Engine::SpriteRegistry> _spriteRegistry = nullptr; ///> Sprite registry for managing sprites

        Network::ClientPacketFactory _packetFactory; ///> Packet factory for creating network packets

        std::unique_ptr<Ecs::PacketRouter> _packetRouter = nullptr;

        Engine::WorldCommandBuffer _commandBuffer; ///> Command buffer for storing commands

        std::mutex _frameMutex;
        std::shared_ptr<const std::vector<Engine::RenderCommand>> _readRenderCommands;
        std::shared_ptr<std::vector<Engine::RenderCommand>> _writeRenderCommands;

        std::thread _receiverThread; ///> Thread for receiving packets
        std::thread _updaterThread;  ///> Thread for updating game state

        std::mutex _mutex;                       ///> Mutex for synchronizing access
        std::condition_variable _cv;             ///> Condition variable for signaling
        std::atomic<bool> _stopRequested{false}; ///> Atomic flag to indicate if stop has been requested
        std::atomic<bool> _running{false};       ///> Atomic flag to indicate if the client is running

        /**
         * @brief Method for running the receiver thread.
         * @details This method continuously receives packets from the network client
         * and routes them for processing.
         */
        void runReceiver() const;

        /**
         * @brief Method for running the updater thread.
         * @details This method updates the game state at a fixed timestep,
         * processes incoming commands, and builds render commands.
         */
        void runUpdater();

        /**
         * @brief Sets up the event registry with key event handlers.
         * @details This method registers key release events to send appropriate
         * input packets to the server.
         */
        void setupEventsRegistry() const;

        /**
         * @brief Processes incoming network packets up to a specified deadline and maximum count.
         * @param deadline The time point by which to stop processing packets.
         * @param maxPackets The maximum number of packets to process.
         * @details This method retrieves packets from the network client and routes them for processing
         * until the deadline is reached or the maximum number of packets has been processed per call.
         */
        void processNetworkPackets(steadyClock::time_point deadline, int maxPackets) const;

        /**
         * @brief Applies world commands from the command buffer up to a specified deadline and maximum count.
         * @param deadline The time point by which to stop applying commands.
         * @param maxCommands The maximum number of commands to apply.
         * @details This method retrieves world commands from the command buffer and applies them
         * to the client world until the deadline is reached or the maximum number of commands has been applied per
         * call.
         */
        void applyWorldCommands(steadyClock::time_point deadline, int maxCommands);

        /**
         *
         */
        void buildAndSwapRenderCommands();
    };

} // namespace Thread
