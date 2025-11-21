/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SignalHandler
*/

#pragma once
#ifdef _WIN32
    #include "WinHandler.hpp"
#else
    #include "PosixHandler.hpp"
#endif
#include "ISignalHandler.hpp"

/**
 * @namespace Signal
 * @brief Signal-related functionalities
 */
namespace Signal
{
    /**
     * @class SignalHandler
     * @brief Cross-platform implementation of the ISignalHandler interface
     * @details This class delegates signal handling to platform-specific implementations.
     */
    class SignalHandler : public ISignalHandler {
      public:
        /**
         * @brief Constructs a new SignalHandler object
         */
        SignalHandler();

        /**
         * @brief Destroys the SignalHandler object
         */
        ~SignalHandler() override = default;

        /**
         * @brief Register a callback for a specific signal type
         * @param type The type of signal to handle
         * @param callback The callback function to execute when the signal is received
         */
        void registerCallback(SignalType type, std::function<void()> callback) noexcept override;

        /**
         * @brief Start the signal handler
         */
        void start() noexcept override;

        /**
         * @brief Stop the signal handler
         */
        void stop() noexcept override;

      private:
        std::unique_ptr<ISignalHandler> _handler;
    };
} // namespace Signal
