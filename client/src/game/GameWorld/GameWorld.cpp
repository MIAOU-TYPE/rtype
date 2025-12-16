/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameWorld
*/

#include "GameWorld.hpp"

namespace Game
{
    GameWorld::GameWorld(Ecs::Registry &registry) : _registry(registry)
    {
        _registry.registerComponent<Ecs::Position>();
        _registry.registerComponent<Ecs::Velocity>();
        _registry.registerComponent<Ecs::Health>();
        _registry.registerComponent<Ecs::InputComponent>();
        _registry.registerComponent<Ecs::NetworkIdentity>();
        _registry.registerComponent<Ecs::NetworkInterpolation>();
        _registry.registerComponent<Ecs::SpriteTag>();
        _registry.registerComponent<Ecs::RenderLink>();
    }

    static std::string pickPlayerSprite(std::size_t id)
    {
        switch (id % 4) {
            case 0: return "player";
            case 1: return "player2";
            case 2: return "player3";
            default: return "player4";
        }
    }

    void GameWorld::update(float dt)
    {
        Command cmd;
        while (_commandBuffer.pop(cmd))
            applyCommand(cmd);

        _registry.view<Ecs::Position, Ecs::NetworkIdentity, Ecs::NetworkInterpolation>(
            [&](Ecs::Entity, auto &pos, auto &netId, auto &interp) {
                if (netId.authoritative)
                    return;

                interp.alpha += dt / interp.snapshotInterval;
                if (interp.alpha > 1.f)
                    interp.alpha = 1.f;

                pos.x = interp.prevX + (interp.targetX - interp.prevX) * interp.alpha;
                pos.y = interp.prevY + (interp.targetY - interp.prevY) * interp.alpha;
            });
    }

    void GameWorld::applyCommand(const Command &cmd)
    {
        switch (cmd.type) {
            case CommandType::CreateEntity: {
                const Ecs::Entity ent = _registry.createEntity();
                _registry.emplaceComponent<Ecs::Position>(ent, cmd.create.x, cmd.create.y);
                _registry.emplaceComponent<Ecs::NetworkIdentity>(ent, cmd.create.id, false);
                _registry.emplaceComponent<Ecs::NetworkInterpolation>(ent);
                _registry.emplaceComponent<Ecs::SpriteTag>(ent, Ecs::SpriteTag{pickPlayerSprite(cmd.create.id)});
                _entityMap[cmd.create.id] = ent;
                std::cout << "cmd.create.it " << cmd.create.id << std::endl;
                std::cout << "static_cast<size_t>(ent)" << static_cast<size_t>(ent) << std::endl;
                break;
            }

            case CommandType::DestroyEntity: {
                const auto it = _entityMap.find(cmd.destroy.id);
                if (it == _entityMap.end())
                    return;
                _registry.destroyEntity(it->second);
                _entityMap.erase(it);
                break;
            }

            case CommandType::Snapshot: {
                const auto netId = cmd.snapshot.entity;

                Ecs::Entity ent;
                const auto it = _entityMap.find(netId);
                const bool isNew = (it == _entityMap.end());
                if (isNew) {
                    ent = _registry.createEntity();
                    _entityMap[netId] = ent;
                } else
                    ent = it->second;

                const size_t idx = static_cast<size_t>(ent);

                const float x = static_cast<float>(cmd.snapshot.x);
                const float y = static_cast<float>(cmd.snapshot.y);

                if (!_registry.getComponents<Ecs::NetworkIdentity>()[idx])
                    _registry.emplaceComponent<Ecs::NetworkIdentity>(ent, netId);

                if (!_registry.getComponents<Ecs::Position>()[idx])
                    _registry.emplaceComponent<Ecs::Position>(ent, x, y);

                if (!_registry.getComponents<Ecs::NetworkInterpolation>()[idx])
                    _registry.emplaceComponent<Ecs::NetworkInterpolation>(ent, x, y, x, y, 1.f);

                if (!_registry.getComponents<Ecs::SpriteTag>()[idx])
                    _registry.emplaceComponent<Ecs::SpriteTag>(ent, Ecs::SpriteTag{"enemy2"});

                auto &pos = *_registry.getComponents<Ecs::Position>()[idx];
                auto &interp = *_registry.getComponents<Ecs::NetworkInterpolation>()[idx];

                if (isNew) {
                    pos.x = x;
                    pos.y = y;
                    interp.prevX = x;
                    interp.prevY = y;
                    interp.targetX = x;
                    interp.targetY = y;
                    interp.alpha = 1.f;
                } else {
                    interp.prevX = pos.x;
                    interp.prevY = pos.y;
                    interp.targetX = x;
                    interp.targetY = y;
                    interp.alpha = 0.f;
                }

                break;
            }

            case CommandType::Accept: std::cout << "{Client} Accepted by server\n"; break;

            case CommandType::Reject: std::cout << "{Client} Rejected by server\n"; break;

            case CommandType::GameOver: std::cout << "{Client} Game over\n"; break;

            default: break;
        }
    }

    void GameWorld::onAccept()
    {
        _commandBuffer.push({CommandType::Accept});
    }

    void GameWorld::onReject()
    {
        _commandBuffer.push({CommandType::Reject});
    }

    void GameWorld::onPong()
    {
    }

    void GameWorld::onGameOver()
    {
        _commandBuffer.push({CommandType::GameOver});
    }

    void GameWorld::onEntityCreate(const EntityCreate &data)
    {
        Command cmd;
        cmd.type = CommandType::CreateEntity;
        cmd.create = data;
        _commandBuffer.push(cmd);
    }

    void GameWorld::onEntityDestroy(const EntityDestroy &data)
    {
        Command cmd;
        cmd.type = CommandType::DestroyEntity;
        cmd.destroy = data;
        _commandBuffer.push(cmd);
    }

    void GameWorld::onSnapshot(const SnapshotEntity &data)
    {
        Command cmd;
        cmd.type = CommandType::Snapshot;
        cmd.snapshot = data;
        _commandBuffer.push(cmd);
    }
} // namespace Game
