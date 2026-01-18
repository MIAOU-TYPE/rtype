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
        const GameConfig &gameConfig, const std::string &name, const size_t maxPlayers)
    {
        Game::DifficultyModifiers modifiers = Game::DifficultyModifiers::fromDifficulty(gameConfig.difficulty);

        _gameServer = std::make_unique<Game::GameServer>(
            sessionManager, udpServer, udpPacketFactory, levelPath, modifiers, gameConfig);
        _roomData.maxPlayers = maxPlayers;
        _roomData.currentPlayers = 0;
        _roomData.roomName = name;
        _roomData.gameConfig = gameConfig;
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
        _gameServer->events().subscribe<GameOverEvent>(
            [udpPacketFactory, sessionManager, udpServer, self](const GameOverEvent &) {
                const auto room = self.lock();
                if (!room)
                    return;
                std::vector<int> sessions;
                {
                    std::scoped_lock lk(room->_sessionsMutex);
                    sessions.assign(room->_sessions.begin(), room->_sessions.end());
                }
                std::vector<std::pair<uint32_t, uint32_t>> finals;
                finals.reserve(sessions.size());
                for (const int sid : sessions) {
                    const uint32_t score = sessionManager->getLastScore(sid).value_or(0);
                    const auto netIdOpt = room->_gameServer->getPlayerNetId(sid);
                    if (!netIdOpt.has_value())
                        continue;
                    finals.emplace_back(*netIdOpt, score);
                }
                const auto endPkt = udpPacketFactory->createGameEndPacket(finals);
                if (!endPkt)
                    return;

                for (int repeat = 0; repeat < 3; ++repeat) {
                    for (const int sid : sessions) {
                        if (const auto addr = sessionManager->getUdpAddress(sid)) {
                            auto clone = endPkt->clone();
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

    void Room::join(const int sessionId, std::string_view username)
    {
        std::scoped_lock lock(_sessionsMutex);

        if (!username.empty() && _bannedUsernames.contains(std::string(username)))
            throw RoomError("{Room::join} username is banned from room");

        if (_sessions.contains(sessionId))
            throw RoomError("{Room::join} session " + std::to_string(sessionId) + " already in room");
        _sessions.insert(sessionId);
        _gameServer->onPlayerConnect(sessionId);
        _roomData.playerNames.emplace_back(username);
    }

    void Room::leave(const int sessionId, std::string_view username)
    {
        std::scoped_lock lock(_sessionsMutex);
        _sessions.erase(sessionId);
        _gameServer->onPlayerDisconnect(sessionId);
        auto &player = _roomData.playerNames;
        std::erase_if(player, [username](const std::string &name) {
            return name == username;
        });
    }

    void Room::banUsername(std::string_view username)
    {
        std::scoped_lock lock(_sessionsMutex);
        if (username.empty())
            return;
        _bannedUsernames.insert(std::string(username));
    }

    void Room::unbanUsername(std::string_view username)
    {
        std::scoped_lock lock(_sessionsMutex);
        _bannedUsernames.erase(std::string(username));
    }

    bool Room::isUsernameBanned(const std::string_view username)
    {
        std::scoped_lock lock(_sessionsMutex);
        return _bannedUsernames.contains(std::string(username));
    }

    bool Room::empty()
    {
        std::scoped_lock lock(_sessionsMutex);
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

    size_t Room::getCurrentPlayers() noexcept
    {
        std::scoped_lock lock(_sessionsMutex);
        return _sessions.size();
    }

    size_t Room::getMaxPlayers() const noexcept
    {
        return _roomData.maxPlayers;
    }

    std::string Room::getName() const noexcept
    {
        return _roomData.roomName;
    }

    std::mutex &Room::getSessionMutex()
    {
        return _sessionsMutex;
    }

    RoomData Room::getRoomData() noexcept
    {
        std::scoped_lock lock(_sessionsMutex);
        _roomData.currentPlayers = _sessions.size();
        return _roomData;
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