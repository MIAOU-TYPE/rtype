/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Room
*/

#include "Room.hpp"

#include <utility>

namespace Engine
{
    Room::Room(const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Net::Server::IServer> &server,
        const std::shared_ptr<Net::Factory::UDPPacketFactory> &udpPacketFactory, const std::string &levelPath,
        std::string name, const size_t maxPlayers)
        : _maxPlayers(maxPlayers), _name(std::move(name))
    {
        _gameServer = std::make_unique<Game::GameServer>(sessions, server, udpPacketFactory, levelPath);
    }

    Room::~Room()
    {
        stop();
    }

    void Room::start()
    {
        _running = true;
        _thread = std::thread(&Room::run, this);
    }

    void Room::stop()
    {
        _running = false;
        if (_thread.joinable())
            _thread.join();
    }

    void Room::join(const int sessionId)
    {
        _sessions.insert(sessionId);
        _gameServer->onPlayerConnect(sessionId);
    }

    void Room::leave(const int sessionId)
    {
        _sessions.erase(sessionId);
        _gameServer->onPlayerDisconnect(sessionId);
    }

    bool Room::empty() const
    {
        return _sessions.empty();
    }

    const std::unordered_set<int> &Room::sessions() const
    {
        return _sessions;
    }

    Game::GameServer &Room::gameServer() const
    {
        return *_gameServer;
    }

    void Room::run() const
    {
        constexpr auto Tick = std::chrono::milliseconds(16);
        auto next = std::chrono::steady_clock::now();

        while (_running) {
            next += Tick;
            _gameServer->tick();
            std::this_thread::sleep_until(next);

            if (auto now = std::chrono::steady_clock::now(); now > next + 5 * Tick)
                next = now;
        }
    }
} // namespace Engine