/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomManager
*/

#pragma once

namespace Engine
{
    template <typename Func>
    void RoomManager::forEachRoom(Func &&func)
    {
        std::vector<std::shared_ptr<Room>> rooms;

        {
            std::scoped_lock lock(_mutex);
            rooms.reserve(_rooms.size());
            for (auto &room : _rooms | std::views::values)
                rooms.push_back(room);
        }

        for (auto &room : rooms)
            func(*room);
    }
} // namespace Engine