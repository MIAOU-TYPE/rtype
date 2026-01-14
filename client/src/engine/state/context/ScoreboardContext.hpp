/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** ScoreboardContext
*/

#pragma once
#include <atomic>
#include <mutex>
#include <vector>
#include "ScoreEntry.hpp"

namespace Engine
{
    /**
     * @struct ScoreboardContext
     * @brief Context structure for managing the scoreboard state.
     */
    struct ScoreboardContext {
        mutable std::mutex m;   ///> Mutex for thread-safe access to the scores.
        std::vector<ScoreEntry> scores; ///> Vector holding the list of score entries.
        std::atomic<uint32_t> version{0};   ///> Atomic version number for tracking updates to the scoreboard.
    };
} // namespace Engine
