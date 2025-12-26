/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientController
*/

#include "ClientController.hpp"

namespace Ecs
{
    ClientController::ClientController(Engine::WorldCommandBuffer &buffer) : _commandBuffer(buffer)
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

    void ClientController::onSnapshot(const std::vector<SnapshotEntity> &data)
    {
        _commandBuffer.get().push({Engine::WorldCommand::Type::Snapshot, data});
    }

    void ClientController::onScore(const uint32_t score)
    {
        _commandBuffer.get().push({Engine::WorldCommand::Type::Score, score});
    }
}; // namespace Ecs
