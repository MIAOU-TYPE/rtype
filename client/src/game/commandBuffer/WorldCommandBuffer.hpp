/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** WorldCommandBuffer
*/

#pragma once
#include <mutex>
#include <queue>
#include <variant>
#include "EntityCreateData.hpp"
#include "EntityDestroyData.hpp"
#include "SnapEntityData.hpp"

struct WorldCommand {
    enum class Type { Accept, Reject, Pong, GameOver, CreateEntity, DestroyEntity, Snapshot };

    Type type;
    std::variant<std::monostate, EntityCreate, EntityDestroy, SnapshotEntity> payload;
};

class WorldCommandBuffer {
  public:
    void push(WorldCommand cmd)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(std::move(cmd));
    }

    bool tryPop(WorldCommand &out)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_queue.empty())
            return false;

        out = std::move(_queue.front());
        _queue.pop();
        return true;
    }

  private:
    std::mutex _mutex;
    std::queue<WorldCommand> _queue;
};
