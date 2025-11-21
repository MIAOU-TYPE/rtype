#pragma once

#ifdef _WIN32

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
     * @class WinHandler
     * @brief Windows implementation of the ISignalHandler interface
     * @details This class handles Windows signals and allows registration of callbacks for specific signal types.
     */
    class WinHandler : public ISignalHandler {
      public:
        /**
         * @brief Constructs a new WinHandler object
         */
        WinHandler();

        /**
         * @brief Destroys the WinHandler object
         */
        ~WinHandler() override;

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
        /**
         * @brief Static signal handler function
         * @param signum The signal number received
         */
        static void handleSignal(int signum);

        static WinHandler *instance; //> Pointer to the singleton instance

        std::atomic_bool running;                              //> Atomic flag to indicate if the handler is running
        std::map<SignalType, std::function<void()>> callbacks; //> Map of signal types to their corresponding callbacks
    };
} // namespace Signal

#endif
