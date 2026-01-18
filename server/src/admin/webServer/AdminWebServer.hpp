/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AdminWebServer
*/

#pragma once
#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <httplib.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#ifndef _WIN32
    #include <arpa/inet.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

#include "ISessionManager.hpp"
#include "RoomManager.hpp"

namespace Net::Admin
{
    /**
     * @class AdminWebServer
     * @brief A simple administrative web server for managing sessions and rooms.
     */
    class AdminWebServer {
      public:
        /**
         * @brief Constructs a new AdminWebServer object.
         * @param sessions Shared pointer to the session manager.
         * @param rooms Shared pointer to the room manager.
         * @param onShutdown Callback function to be called on server shutdown.
         * @param port Port number for the web server (default is 8082).
         * @param token Authentication token for accessing admin endpoints (default is "admin").
         */
        AdminWebServer(std::shared_ptr<Server::ISessionManager> sessions, std::shared_ptr<Engine::RoomManager> rooms,
            std::function<void()> onShutdown, int port = 8082, std::string token = "admin");

        /**
         * @brief Destroys the AdminWebServer object and stops the server if running.
         */
        ~AdminWebServer();

        /**
         * @brief Starts the admin web server in a separate thread.
         */
        void start();

        /**
         * @brief Stops the admin web server.
         */
        void stop() noexcept;

        /**
         * @brief Gets the port number the server is running on.
         * @return The port number.
         */
        [[nodiscard]] int port() const noexcept;

      private:
        void run(); ///> Thread function to run the web server

        std::shared_ptr<Server::ISessionManager> _sessions; ///> Session manager
        std::shared_ptr<Engine::RoomManager> _rooms;        ///> Room manager
        std::function<void()> _onShutdown;                  ///> Shutdown callback
        std::unique_ptr<httplib::Server> _srv;              ///> HTTP server instance

        int _port = 8082;   ///> Server port
        std::string _token; ///> Authentication token

        std::atomic<bool> _running{false}; ///> Flag indicating if the server is running
        std::thread _thread;               ///> Thread for the web server
    };
} // namespace Net::Admin
