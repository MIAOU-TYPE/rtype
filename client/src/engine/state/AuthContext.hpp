/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AuthContext
*/

#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>

namespace Engine
{
    /**
     * @struct AuthContext
     * @brief Holds authentication context information for a user.
     *
     * This structure contains information about the authentication status,
     * user ID, token, time-to-live (TTL), and username. It uses atomic
     * operations for thread-safe access to the authentication status.
     */
    struct AuthContext {
        std::atomic_bool authed{false}; ///> Indicates whether the user is authenticated

        std::atomic_uint32_t authErrorVersion{
            0};                   ///> Bumped every time authError changes so UI can poll without registering handlers.
        std::mutex m;             ///> Mutex for protecting access to user-related data
        std::uint32_t userId = 0; ///> Unique identifier for the user
        std::uint64_t token = 0;  ///> Authentication token
        std::uint32_t ttlSec = 0; ///> Time-to-live for the authentication token in seconds
        std::string username;     ///> Username of the authenticated user
        std::string authError;    ///> Last authentication error message (login/register)
    };
} // namespace Engine
