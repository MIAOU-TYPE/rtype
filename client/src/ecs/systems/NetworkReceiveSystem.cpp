/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetworkReceiveSystem
*/

#include "NetworkReceiveSystem.hpp"
#include "HeaderData.hpp"

namespace Ecs
{
    NetworkReceiveSystem::NetworkReceiveSystem(
        Game::NetClient &client, std::shared_ptr<Command::CommandBuffer<Game::NetworkCommand>> commandBuffer)
        : _netClient(client), _commandBuffer(std::move(commandBuffer))
    {
    }

    void NetworkReceiveSystem::update(float deltaTime)
    {
        (void) deltaTime;

        Game::NetworkCommand cmd;
        while (_commandBuffer->pop(cmd)) {
            handleCommand(cmd);
        }
    }

    void NetworkReceiveSystem::handleCommand(const Game::NetworkCommand &cmd)
    {
        if (cmd.data.size() < sizeof(HeaderData)) {
            return;
        }

        const auto *header = reinterpret_cast<const HeaderData *>(cmd.data.data());
        switch (header->type) {
            case Net::Protocol::ACCEPT: {
                _netClient.setConnected(true);
                if (cmd.data.size() >= sizeof(HeaderData) + sizeof(uint32_t)) {
                    const auto *entityId =
                        reinterpret_cast<const uint32_t *>(cmd.data.data() + sizeof(HeaderData));
                    _netClient.setPlayerEntityId(*entityId);
                }
                break;
            }

            case Net::Protocol::REJECT: {
                _netClient.setConnected(false);
                break;
            }

            case Net::Protocol::SNAPSHOT: {
                // TODO: Handle game state snapshot
                break;
            }

            case Net::Protocol::ENTITY_CREATE: {
                // TODO: Handle entity creation
                break;
            }

            case Net::Protocol::ENTITY_DESTROY: {
                // TODO: Handle entity destruction
                break;
            }

            case Net::Protocol::PONG: {
                // Ping response received - no action needed
                break;
            }

            case Net::Protocol::DAMAGE_EVENT: {
                // TODO: Handle damage event
                break;
            }

            case Net::Protocol::GAME_OVER: {
                // TODO: Handle game over
                break;
            }

            default: break;
        }
    }
} // namespace Ecs