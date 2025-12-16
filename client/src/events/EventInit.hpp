/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** EventInit
*/

#pragma once

#include <iostream>
#include <memory>
#include "ClientPacketFactory.hpp"
#include "INetClient.hpp"
#include "IRenderer.hpp"
#include "InputData.hpp"
#include "SFMLEvent.hpp"

/**
 * @namespace Events
 * @brief Contains all event initialization and management classes.
 */
namespace Events
{
    /**
     * @class EventInitError
     * @brief Exception class for event-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class EventInitError : public std::exception {
      public:
        /**
         * @brief Constructor for EventInitError.
         * @param message The error message to be associated with the exception.
         */
        explicit EventInitError(const std::string &message) : _message("\n\t" + message)
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
        std::string _message = ""; //> Error message
    };

    /**
     * @class EventInit
     * @brief Manages event processing for the client.
     *
     * This class handles polling and processing of events from the renderer.
     * It's designed to be used in a separate thread alongside DisplayInit,
     * sharing the same renderer instance with thread-safe access.
     */
    class EventInit {
      public:
        /**
         * @brief Constructor for EventInit.
         * @param renderer Shared pointer to the renderer interface.
         */
        explicit EventInit(std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Network::INetClient> client,
            std::shared_ptr<Network::ClientPacketFactory>);

        /**
         * @brief Destructor for EventInit.
         */
        ~EventInit() = default;

        /**
         * @brief Processes one cycle of event polling and handling.
         * Should be called repeatedly in a loop.
         */
        void run();

        /**
         * @brief Checks if the window is still open.
         * @return True if the window is open, false otherwise.
         */
        bool isWindowOpen() const;

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr; ///> The renderer interface
        std::shared_ptr<Network::INetClient> _client = nullptr;   ///> The network client
        std::shared_ptr<Network::ClientPacketFactory> _packetFactory = nullptr;
    };
} // namespace Events
