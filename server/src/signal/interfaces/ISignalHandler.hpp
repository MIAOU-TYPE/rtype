/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ISignalHandler
*/

#pragma once

#include <functional>
#include <mutex>
#include <vector>

/**
 * @namespace Signal
 * @brief Signal-related functionalities
 */
namespace Signal
{
    /**
     * @enum SignalType
     * @brief Types of signals that can be handled
     */
    enum class SignalType { Interrupt, Terminate, Hangup };

    /**
     * @interface ISignalHandler
     * @brief Interface for handling system signals
     */
    class ISignalHandler {
      public:
        /**
         * @brief Virtual destructor
         */
        virtual ~ISignalHandler() = default;

        /**
         * @brief Register a callback for a specific signal type
         * @param type The type of signal to handle
         * @param callback The callback function to execute when the signal is received
         */
        virtual void registerCallback(SignalType type, std::function<void()> callback) noexcept = 0;

        /**
         * @brief Start the signal handler
         */
        virtual void start() noexcept = 0;

        /**
         * @brief Stop the signal handler
         */
        virtual void stop() noexcept = 0;
    };
} // namespace Signal
