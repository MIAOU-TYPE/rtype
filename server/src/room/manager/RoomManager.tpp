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
        std::vector<Room *> rooms;

        {
            std::scoped_lock lock(_mutex);
            for (auto &room : _rooms | std::views::values)
                rooms.push_back(room.get());
        }

        for (Room *room : rooms)
            func(*room);
    }
} // namespace Engine