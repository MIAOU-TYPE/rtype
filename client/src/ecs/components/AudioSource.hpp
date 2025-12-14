/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AudioSource
*/

#pragma once
#include <string>

namespace Ecs
{
    /**
     * @struct AudioSource
     * @brief Component for playing sounds on an entity.
     *
     * Works with an AudioSystem that manages playback via SFML.
     */

    struct AudioSource {
        std::string soundKey = ""; ///> Sound identifier
        bool loop = false;         ///> Play in loop
        float volume = 100.f;      ///> Volume (0-100)
        float pitch = 1.0f;        ///> Playback speed
        bool playOnce = true;      ///> Destroy after playback
        bool isPlaying = false;    ///> Playback state
        bool spatial = false;      ///> 3D sound (distance)
        float minDistance = 100.f; ///> Minimum distance for attenuation
    };
} // namespace Ecs