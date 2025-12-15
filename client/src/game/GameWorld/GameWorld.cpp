/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameWorld
*/

#include "GameWorld.hpp"

#include "Position.hpp"
#include "Velocity.hpp"
#include "Health.hpp"
#include "NetworkSync.hpp"
#include <iostream>

namespace Game
{
    GameWorld::GameWorld(Ecs::Registry &registry)
        : _registry(registry)
    {
        _registry.registerComponent<Ecs::Position>();
        _registry.registerComponent<Ecs::Velocity>();
        _registry.registerComponent<Ecs::Health>();
        _registry.registerComponent<Ecs::NetworkSync>();
    }

    void GameWorld::update(float)
    {
        Command cmd;
        while (_commandBuffer.pop(cmd)) {
            applyCommand(cmd);
        }
    }

    void GameWorld::applyCommand(const Command &cmd)
    {
        switch (cmd.type) {

            case CommandType::CreateEntity: {
                Ecs::Entity ent = _registry.createEntity();

                _registry.emplaceComponent<Ecs::Position>(
                    ent, cmd.create.x, cmd.create.y);

                _registry.emplaceComponent<Ecs::NetworkSync>(
                    ent, cmd.create.id); // <-- ID serveur réel

                _entityMap[cmd.create.id] = ent;
                break;
            }

            case CommandType::DestroyEntity: {
                auto it = _entityMap.find(cmd.destroy.id);
                if (it == _entityMap.end())
                    return;

                _registry.destroyEntity(it->second);
                _entityMap.erase(it);
                break;
            }

            case CommandType::Accept:
                std::cout << "[Client] Accepted by server\n";
                break;

            case CommandType::Reject:
                std::cout << "[Client] Rejected by server\n";
                break;

            case CommandType::GameOver:
                std::cout << "[Client] Game over\n";
                break;

            default:
                break;
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

    void GameWorld::onPong() {}

    void GameWorld::onGameOver()
    {
        _commandBuffer.push({CommandType::GameOver});
    }

    void GameWorld::onEntityCreate(const EntityCreateData &data)
    {
        Command cmd;
        cmd.type = CommandType::CreateEntity;
        cmd.create = data;
        _commandBuffer.push(cmd);
    }

    void GameWorld::onEntityDestroy(const EntityDestroyData &data)
    {
        Command cmd;
        cmd.type = CommandType::DestroyEntity;
        cmd.destroy = data;
        _commandBuffer.push(cmd);
    }
}
