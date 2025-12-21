/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** WorldCommandBuffer
*/

#include "WorldCommandBuffer.hpp"

namespace Engine
{
    void WorldCommandBuffer::push(const WorldCommand &cmd)
    {
        std::scoped_lock lock(_mutex);
        _queue.push(cmd);
    }

    bool WorldCommandBuffer::tryPop(WorldCommand &out)
    {
        std::scoped_lock lock(_mutex);
        if (_queue.empty())
            return false;

        out = _queue.front();
        _queue.pop();
        return true;
    }
} // namespace Engine