/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CommandBuffer
*/

#pragma once
#include "CommandBuffer.hpp"

namespace Command
{
    template <typename T>
    bool CommandBuffer<T>::push(const T &cmd) noexcept
    {
        std::scoped_lock lock(_mutex);
        _queue.push(cmd);
        return true;
    }

    template <typename T>
    bool CommandBuffer<T>::pop(T &out) noexcept
    {
        std::scoped_lock lock(_mutex);
        if (_queue.empty())
            return false;
        out = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    template <typename T>
    void CommandBuffer<T>::clear() noexcept
    {
        std::scoped_lock lock(_mutex);
        std::queue<T> empty;
        _queue.swap(empty);
    }

    template <typename T>
    bool CommandBuffer<T>::empty() const noexcept
    {
        std::scoped_lock lock(_mutex);
        return _queue.empty();
    }
} // namespace Command
