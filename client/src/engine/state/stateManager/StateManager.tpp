/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** StateManager
*/

#pragma once

namespace Engine
{
    template <class T>
    bool StateManager::is() const noexcept
    {
        return dynamic_cast<const T *>(_current.get()) != nullptr;
    }
} // namespace Engine