#pragma once

#ifndef _WIN32

    #include <atomic>
    #include <map>
    #include <signal.h>
    #include "ISignalHandler.hpp"

/**
 * @namespace Signal
 * @brief Signal-related functionalities
 */
namespace Signal
{
    /**
     * @class PosixHandler
     * @brief POSIX implementation of the ISignalHandler interface
     * @details This class handles POSIX signals and allows registration of callbacks for specific signal types.
     */
    class PosixHandler : public ISignalHandler {
      public:
        /**
         * @brief Constructs a new PosixHandler object
         */
        PosixHandler();

        /**
         * @brief Destroys the PosixHandler object
         */
        ~PosixHandler() override;

        /**
         * @brief Register a callback for a specific signal type
         * @param type The type of signal to handle
         * @param callback The callback function to execute when the signal is received
         */
        void registerCallback(SignalType type, std::function<void()> callback) override;

        /**
         * @brief Start the signal handler
         */
        void start() override;

        /**
         * @brief Stop the signal handler
         */
        void stop() override;

      private:
        /**
         * @brief Static signal handler function
         * @param sig The signal number received
         */
        static void handleSignal(int sig);

        /**
         * @brief Dispatch the signal to the registered handlers
         * @param type The type of signal to dispatch
         */
        void dispatch(SignalType type);

        static PosixHandler *instance;                                      //> Pointer to the singleton instance
        std::map<SignalType, std::vector<std::function<void()>>> _handlers; //> Map of signal handlers

        std::mutex mutex;                 //> Mutex for thread-safe access to handlers
        std::atomic<bool> running{false}; //> Atomic flag indicating if the handler is running
    };
} // namespace Signal

#endif
