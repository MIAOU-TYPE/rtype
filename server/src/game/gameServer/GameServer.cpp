/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameServer
*/

#include "GameServer.hpp"
#include "UDPPacket.hpp"

namespace Game
{
    GameServer::GameServer(std::shared_ptr<Net::Server::ISessionManager> sessions,
        std::shared_ptr<Net::Server::IServer> server, std::shared_ptr<Net::Factory::PacketFactory> packetFactory)
        : _world(std::make_unique<World>()), _sessions(std::move(sessions)), _server(std::move(server)),
          _packetFactory(std::move(packetFactory))
    {
    }

    void GameServer::onPlayerConnect(const int sessionId)
    {
        GameCommand cmd;
        cmd.type = GameCommand::Type::PlayerConnect;
        cmd.sessionId = sessionId;
        _commandBuffer.push(cmd);
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

    void GameServer::update(const float dt) const
    {
        InputSystem::update(*_world);
        ShootingSystem::update(*_world);
        AISystem::update(*_world, dt);
        MovementSystem::update(*_world, dt);
        EnemySpawnSystem::update(*_world, dt);
        CollisionSystem::update(*_world);
        DamageSystem::update(*_world);
        HealthSystem::update(*_world);
        LifetimeSystem::update(*_world, dt);
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
            _accumulator -= FIXED_DT;
        }
    }

    void GameServer::buildSnapshot(std::vector<SnapshotEntity> &out) const
    {
        out.clear();
        SnapshotSystem::update(*_world, out);
    }

    void GameServer::applyCommand(const GameCommand &cmd)
    {
        switch (cmd.type) {
            case GameCommand::Type::PlayerConnect: {
                const Ecs::Entity ent = _world->createPlayer();
                _sessionToEntity[cmd.sessionId] = ent;
                break;
            }
            case GameCommand::Type::PlayerDisconnect: {
                if (!_sessionToEntity.contains(cmd.sessionId))
                    break;
                const Ecs::Entity ent = _sessionToEntity[cmd.sessionId];
                _world->destroyEntity(ent);
                _sessionToEntity.erase(cmd.sessionId);
                break;
            }
            case GameCommand::Type::PlayerInput: {
                if (!_sessionToEntity.contains(cmd.sessionId))
                    break;
                const Ecs::Entity ent = _sessionToEntity[cmd.sessionId];
                auto &inputArr = _world->registry().getComponents<InputComponent>();
                auto &inputOpt = inputArr[static_cast<size_t>(ent)];
                if (inputOpt.has_value())
                    *inputOpt = cmd.input;
                break;
            }
            case GameCommand::Type::Ping: {
                if (const auto *addr = _sessions->getAddress(cmd.sessionId)) {
                    if (const auto pkt = _packetFactory->makeDefault(*addr, Net::Protocol::PONG))
                        _server->sendPacket(*pkt);
                }
                break;
            }
            default: break;
        }
    }
} // namespace Game
