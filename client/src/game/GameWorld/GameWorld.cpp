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
                std::cout << cmd.create.id << std::endl;
                std::cout << static_cast<size_t>(ent) << std::endl;
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

                auto it = _entityMap.find(netId);
                if (it == _entityMap.end()) {
                    const Ecs::Entity enti = _registry.createEntity();
                    _entityMap[netId] = enti;
                    it = _entityMap.find(netId);

                    _registry.emplaceComponent<Ecs::NetworkIdentity>(enti, netId);

                    constexpr float SNAPSHOT_SCALE = 1000.f;
                    _registry.emplaceComponent<Ecs::Position>(enti, static_cast<float>(cmd.snapshot.x) / SNAPSHOT_SCALE,
                        static_cast<float>(cmd.snapshot.y) / SNAPSHOT_SCALE);

                    _registry.emplaceComponent<Ecs::NetworkInterpolation>(enti,
                        /*prevX*/ 0.f, /*prevY*/ 0.f, /*targetX*/ 0.f, /*targetY*/ 0.f, /*alpha*/ 0.f);
                    _registry.emplaceComponent<Ecs::SpriteTag>(enti, Ecs::SpriteTag{"enemy2"});
                }

                const Ecs::Entity ent = it->second;

                auto &posOpt = _registry.getComponents<Ecs::Position>()[static_cast<size_t>(ent)];
                auto &interpOpt = _registry.getComponents<Ecs::NetworkInterpolation>()[static_cast<size_t>(ent)];

                if (!posOpt) {
                    constexpr float SNAPSHOT_SCALE = 1000.f;
                    _registry.emplaceComponent<Ecs::Position>(ent, static_cast<float>(cmd.snapshot.x) / SNAPSHOT_SCALE,
                        static_cast<float>(cmd.snapshot.y) / SNAPSHOT_SCALE);
                    posOpt = _registry.getComponents<Ecs::Position>()[static_cast<size_t>(ent)];
                }
                if (!interpOpt) {
                    _registry.emplaceComponent<Ecs::NetworkInterpolation>(ent, 0.f, 0.f, 0.f, 0.f, 0.f);
                    interpOpt = _registry.getComponents<Ecs::NetworkInterpolation>()[static_cast<size_t>(ent)];
                }

                auto &pos = *posOpt;
                auto &interp = *interpOpt;

                interp.prevX = pos.x;
                interp.prevY = pos.y;

                constexpr float SNAPSHOT_SCALE = 1000.f;
                interp.targetX = static_cast<float>(cmd.snapshot.x) / SNAPSHOT_SCALE;
                interp.targetY = static_cast<float>(cmd.snapshot.y) / SNAPSHOT_SCALE;
                interp.alpha = 0.f;

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
