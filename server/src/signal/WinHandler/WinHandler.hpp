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
     * @brief POSIX implementation of the ISignalHandler interface
     * @details This class handles POSIX signals and allows registration of callbacks for specific signal types.
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
        static WinHandler *instance;
        static void handleSignal(int signum);

        std::atomic_bool running;
        std::map<SignalType, std::function<void()>> callbacks;
    };
} // namespace Signal

#endif
