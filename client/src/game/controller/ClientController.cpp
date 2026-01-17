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
        _gameOverQueued.store(false);
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
        if (_gameOverQueued.exchange(true))
            return;
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

    void ClientController::onScore(const uint32_t playerId, const uint32_t score)
    {
        World::WorldCommand cmd;
        cmd.type = World::WorldCommand::Type::Score;
        cmd.payload = World::ScoreUpdate{playerId, score};
        _commandBuffer.get().push(cmd);
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
} // namespace Ecs
