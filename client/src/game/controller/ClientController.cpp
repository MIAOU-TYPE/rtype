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

    void ClientController::onAccept(const uint32_t sessionId)
    {
        _commandBuffer.get().push(World::WorldCommand{
            .type = World::WorldCommand::Type::Accept,
            .payload = sessionId,
        });
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
        _commandBuffer.get().push({World::WorldCommand::Type::GameOver, {}});
    }

    void ClientController::onSnapshot(const uint32_t serverTick, const std::vector<SnapshotEntity> &data)
    {
        World::SnapshotBatch batch;
        batch.serverTick = serverTick;
        batch.entities = data;

        _commandBuffer.get().push(World::WorldCommand{
            .type = World::WorldCommand::Type::Snapshot,
            .payload = std::move(batch),
        });
    }

    void ClientController::onScore(const uint32_t score)
    {
        _commandBuffer.get().push({World::WorldCommand::Type::Score, score});
    }

    void ClientController::onDamage(const size_t targetId, const bool wasKilled)
    {
        _commandBuffer.get().push({World::WorldCommand::Type::Damage, World::DamageInfo{targetId, wasKilled}});
    }

    void ClientController::onDestroy(const size_t entityId, const bool wasKilled)
    {
        _commandBuffer.get().push({World::WorldCommand::Type::Destroy, World::DestroyInfo{entityId, wasKilled}});
    }

    void ClientController::onHealth(const uint16_t currentLife, const uint16_t maxLife)
    {
        std::cout << "onHealth: " << currentLife << " / " << maxLife << std::endl;
    }
}; // namespace Ecs
