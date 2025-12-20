/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientController
*/

#include "ClientController.hpp"

#include "Drawable.hpp"
#include "Position.hpp"

namespace Ecs
{
    ClientController::ClientController(WorldCommandBuffer &buffer) : _commandBuffer(buffer)
    {
    }

    void ClientController::onAccept()
    {
        std::cout << "onAccept" << std::endl;
    }

    void ClientController::onReject()
    {
        std::cout << "onReject" << std::endl;
    }

    void ClientController::onPong()
    {
        std::cout << "onPong" << std::endl;
    }

    void ClientController::onGameOver()
    {
        std::cout << "onGameOver" << std::endl;
    }

    void ClientController::onEntityCreate(const EntityCreate &data)
    {
        _commandBuffer.push({WorldCommand::Type::CreateEntity, data});
    }

    void ClientController::onEntityDestroy(const EntityDestroy &data)
    {
        _commandBuffer.push({WorldCommand::Type::DestroyEntity, data});
    }

    void ClientController::onSnapshot(const SnapshotEntity &data)
    {
        _commandBuffer.push({WorldCommand::Type::Snapshot, data});
    }

}; // namespace Ecs
