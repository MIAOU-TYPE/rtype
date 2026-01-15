/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Room
*/

#include "Room.hpp"

namespace Engine
{
    Room::Room(const std::shared_ptr<Net::Server::ISessionManager> &sessionManager,
        const std::shared_ptr<Net::Server::IServer> &udpServer,
        const std::shared_ptr<Net::Factory::UDPPacketFactory> &udpPacketFactory, const std::string &levelPath,
        const Engine::GameConfig &gameConfig, std::string name, const size_t maxPlayers)
        : _maxPlayers(maxPlayers), _name(std::move(name)), _gameConfig(gameConfig)
    {
        Game::DifficultyModifiers modifiers = Game::DifficultyModifiers::fromDifficulty(gameConfig.difficulty);

        _gameServer =
            std::make_unique<Game::GameServer>(sessionManager, udpServer, udpPacketFactory, levelPath, modifiers);
    }

    void Room::init(const std::shared_ptr<Net::Server::ISessionManager> &sessionManager,
        const std::shared_ptr<Net::Server::IServer> &udpServer,
        const std::shared_ptr<Net::Factory::UDPPacketFactory> &udpPacketFactory)
    {
        auto self = weak_from_this();

        _gameServer->events().subscribe<DestroyEvent>(
            [udpPacketFactory, sessionManager, udpServer, self](const DestroyEvent &data) {
                if (const auto room = self.lock()) {
                    std::scoped_lock lock(room->_sessionsMutex);
                    const auto out = udpPacketFactory->createDestroyEntityPacket(data.entityId, data.wasKilled);
                    if (!out)
                        return;
                    for (const auto &player : room->_sessions) {
                        if (const auto addr = sessionManager->getUdpAddress(player)) {
                            auto clone = out->clone();
                            clone->setAddress(*addr);
                            (void) udpServer->sendPacket(*clone);
                        }
                    }
                }
            });
    }

    Room::~Room()
    {
        stop();
    }

    void Room::start()
    {
        if (_thread.joinable())
            return;
        _running = true;
        _gameServer->reset();
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
        if (_sessions.contains(sessionId))
            throw RoomError("{Room::join} session " + std::to_string(sessionId) + " already in room");
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

    size_t Room::getCurrentPlayers() const noexcept
    {
        return _sessions.size();
    }

    size_t Room::getMaxPlayers() const noexcept
    {
        return _maxPlayers;
    }

    std::string Room::getName() const noexcept
    {
        return _name;
    }

    const Engine::GameConfig &Room::getGameConfig() const noexcept
    {
        return _gameConfig;
    }

    std::mutex &Room::getSessionMutex()
    {
        return _sessionsMutex;
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