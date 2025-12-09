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
    GameServer::GameServer(
        std::shared_ptr<Net::Server::ISessionManager> sessions, std::shared_ptr<Net::Server::IServer> server)
        : _world(std::make_unique<World>()), _sessions(std::move(sessions)), _server(std::move(server)),
          _factory(std::make_shared<Net::UDPPacket>())
    {
    }

    void GameServer::onPlayerConnect(const int sessionId)
    {
        const Ecs::Entity ent = _world->createPlayer();
        _sessionToEntity[sessionId] = ent;
    }

    void GameServer::onPlayerDisconnect(const int sessionId)
    {
        if (!_sessionToEntity.contains(sessionId))
            return;

        const Ecs::Entity ent = _sessionToEntity[sessionId];
        _world->destroyEntity(ent);
        _sessionToEntity.erase(sessionId);
    }

    void GameServer::onPlayerInput(const int sessionId, const InputComponent &msg)
    {
        if (!_sessionToEntity.contains(sessionId))
            return;

        const Ecs::Entity ent = _sessionToEntity[sessionId];
        if (auto &input = _world->registry().getComponents<InputComponent>()[static_cast<size_t>(ent)]) {
            *input = msg;
        }
    }

    void GameServer::onPing(const int sessionId)
    {
        if (const auto *ai = _sessions->getAddress(sessionId)) {
            if (const auto pkt = _factory.makeDefault(*ai, Net::Protocol::PONG))
                _server->sendPacket(*pkt);
        }
    }

    void GameServer::update(const float dt) const
    {
        InputSystem::update(*_world);
        MovementSystem::update(*_world, dt);
    }
} // namespace Game
