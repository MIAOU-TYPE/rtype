/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientController
*/

#include "ClientController.hpp"

namespace Ecs
{
    ClientController::ClientController(Command::CommandBuffer<World::WorldCommand> &buffer) : _commandBuffer(buffer)
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
        _commandBuffer.get().push({World::WorldCommand::Type::Snapshot, data});
    }

    void ClientController::onScore(const uint32_t score)
    {
        std::cout << "onScore: " << score << std::endl;
    }

    void ClientController::onDamage(const uint32_t targetId, const uint16_t amount, const bool wasKilled)
    {
        _commandBuffer.get().push({World::WorldCommand::Type::Damage, World::DamageInfo{targetId, amount, wasKilled}});
    }

    void ClientController::onDestroy(const size_t entityId, const bool wasKilled)
    {
        _commandBuffer.get().push({World::WorldCommand::Type::Destroy, World::DestroyInfo{entityId, wasKilled}});
    }
}; // namespace Ecs
