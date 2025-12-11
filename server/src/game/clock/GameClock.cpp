/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameClock
*/

#include "GameClock.hpp"

namespace Game
{
    GameClock::GameClock() : _last(std::chrono::steady_clock::now())
    {
    }

    double GameClock::restart()
    {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<double> diff = now - _last;
        _last = now;
        return diff.count();
    }

    double GameClock::elapsed() const
    {
        const auto now = std::chrono::steady_clock::now();
        return std::chrono::duration<double>(now - _last).count();
    }
} // namespace Game