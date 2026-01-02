/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameServer
*/

#include "GameServer.hpp"

namespace
{
    void registerScoreUpdatePacketDispatch(Game::IGameWorld &world,
        const std::shared_ptr<Net::Server::ISessionManager> &sessions,
        const std::shared_ptr<Net::Factory::UDPPacketFactory> &UDPPacketFactory,
        const std::unordered_map<size_t, int> &entityToSession, const std::shared_ptr<Net::Server::IServer> &server)
    {
        std::weak_ptr<Net::Server::ISessionManager> wSessions = sessions;
        std::weak_ptr<Net::Factory::UDPPacketFactory> wFactory = UDPPacketFactory;
        std::weak_ptr<Net::Server::IServer> wServer = server;

        const auto *mapPtr = &entityToSession;

        world.events().subscribe<ScoreUpdatedEvent>(
            [wSessions, wFactory, wServer, mapPtr](const ScoreUpdatedEvent &scoreUpdated) {
                const auto sessionsL = wSessions.lock();
                const auto factoryL = wFactory.lock();
                const auto serverL = wServer.lock();
                if (!sessionsL || !factoryL || !serverL || !mapPtr)
                    return;

                const auto it = mapPtr->find(scoreUpdated.playerId);
                if (it == mapPtr->end())
                    return;

                const int sessionId = it->second;
                const sockaddr_in *addr = sessionsL->getAddress(sessionId);
                if (!addr)
                    return;

                const auto totalScore = static_cast<uint32_t>(scoreUpdated.newScore);
                if (const auto pkt = factoryL->createScorePacket(*addr, totalScore))
                    serverL->sendPacket(*pkt);
            });
    }
} // namespace

namespace Game
{
    GameServer::GameServer(std::shared_ptr<Net::Server::ISessionManager> sessions,
        std::shared_ptr<Net::Server::IServer> server, std::shared_ptr<Net::Factory::UDPPacketFactory> udpPacketFactory,
        const std::string &levelPath)
        : _worldWrite(std::make_unique<World>()), _worldRead(std::make_unique<World>()),
          _worldTemp(std::make_unique<World>()), _sessions(std::move(sessions)), _server(std::move(server)),
          _udpPacketFactory(std::move(udpPacketFactory))
    {
        if (!levelPath.empty()) {
            if (!_levelManager.loadFromFile(levelPath))
                std::cerr << "{GameServer::GameServer} Failed to load level file: " << levelPath << "\n";
            else
                std::cout << "{GameServer::GameServer} Loaded level: " << _levelManager.getCurrentLevel().name << "\n";
            _levelManager.reset();
        }
        _waitingClock.restart();

        registerScoreUpdatePacketDispatch(*_worldWrite, _sessions, _udpPacketFactory, _entityToSession, _server);
    }

    void GameServer::onPlayerConnect(const int sessionId)
    {
        GameCommand cmd;
        cmd.type = GameCommand::Type::PlayerConnect;
        cmd.sessionId = sessionId;
        _commandBuffer.push(cmd);
        if (const auto *addr = _sessions->getAddress(sessionId)) {
            _server->sendPacket(*_udpPacketFactory->makeDefault(*addr, Net::Protocol::UDP::ACCEPT));
        }
    }

    void GameServer::onPlayerDisconnect(const int sessionId)
    {
        GameCommand cmd;
        cmd.type = GameCommand::Type::PlayerDisconnect;
        cmd.sessionId = sessionId;
        _commandBuffer.push(cmd);
    }

    void GameServer::onPlayerInput(const int sessionId, const InputComponent &msg)
    {
        GameCommand cmd;
        cmd.type = GameCommand::Type::PlayerInput;
        cmd.sessionId = sessionId;
        cmd.input = msg;
        _commandBuffer.push(cmd);
    }

    void GameServer::onPing(const int sessionId)
    {
        GameCommand cmd;
        cmd.type = GameCommand::Type::Ping;
        cmd.sessionId = sessionId;
        _commandBuffer.push(cmd);
    }

    void GameServer::update(const float dt)
    {
        if (_waitingClock.elapsed() > 5.0)
            LevelSystem::update(*_worldWrite, _levelManager, dt, _spawned);

        AIShootSystem::update(*_worldWrite, dt);

        InputSystem::update(*_worldWrite);
        ShootingSystem::update(*_worldWrite);

        MovementSystem::update(*_worldWrite, dt);
        CollisionSystem::update(*_worldWrite);
        HealthSystem::update(*_worldWrite);
        LifetimeSystem::update(*_worldWrite, dt);

        _worldWrite->events().process();
    }

    void GameServer::tick()
    {
        GameCommand cmd;
        while (_commandBuffer.pop(cmd))
            applyCommand(cmd);
        const double frameTime = _clock.restart();
        _accumulator += frameTime;
        while (_accumulator >= FIXED_DT) {
            update(static_cast<float>(FIXED_DT));
            _worldTemp->copyFrom(*_worldWrite);
            {
                std::scoped_lock lock(_snapshotMutex);
                std::swap(_worldTemp, _worldRead);
            }
            _accumulator -= FIXED_DT;
        }
    }

    void GameServer::buildSnapshot(std::vector<SnapshotEntity> &out) const
    {
        out.clear();
        std::scoped_lock lock(_snapshotMutex);
        SnapshotSystem::update(*_worldRead, out);
    }

    void GameServer::applyCommand(const GameCommand &cmd)
    {
        switch (cmd.type) {
            case GameCommand::Type::PlayerConnect: {
                const Ecs::Entity ent = _worldWrite->createPlayer();
                _sessionToEntity[cmd.sessionId] = ent;
                _entityToSession[static_cast<size_t>(ent)] = cmd.sessionId;
                break;
            }

            case GameCommand::Type::PlayerDisconnect: {
                const auto it = _sessionToEntity.find(cmd.sessionId);
                if (it == _sessionToEntity.end())
                    break;
                const Ecs::Entity ent = it->second;
                _entityToSession.erase(static_cast<size_t>(ent));
                _sessionToEntity.erase(it);
                _sessions->removeSession(cmd.sessionId);
                _worldWrite->destroyEntity(ent);
                break;
            }
            case GameCommand::Type::PlayerInput: {
                if (!_sessionToEntity.contains(cmd.sessionId))
                    break;
                const Ecs::Entity ent = _sessionToEntity[cmd.sessionId];
                auto &inputs = _worldWrite->registry().getComponents<InputComponent>();

                if (auto &inputOpt = inputs.at(static_cast<size_t>(ent)); inputOpt.has_value()) {
                    inputOpt->up = cmd.input.up;
                    inputOpt->down = cmd.input.down;
                    inputOpt->left = cmd.input.left;
                    inputOpt->right = cmd.input.right;
                    inputOpt->shoot = cmd.input.shoot;
                }
                break;
            }
            case GameCommand::Type::Ping: {
                if (const auto *addr = _sessions->getAddress(cmd.sessionId)) {
                    if (const auto pkt = _udpPacketFactory->makeDefault(*addr, Net::Protocol::UDP::PONG))
                        _server->sendPacket(*pkt);
                }
                break;
            }
            default: break;
        }
    }
} // namespace Game
