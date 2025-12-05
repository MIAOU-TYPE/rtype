/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Animation
*/

#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>

namespace Ecs
{
    /**
     * @struct Animation
     * @brief Component for handling sprite animations.
     * This component manages a sequence of frames for animating sprites,
     * including frame duration, looping, and playback state.
     */
    struct Animation {
        std::vector<sf::IntRect> frames;  // List of texture rectangles
        size_t currentFrame = 0;          // Current frame index
        float frameDuration = 0.1f;       // Duration of a frame (seconds)
        float elapsedTime = 0.f;          // Elapsed time in the current frame
        bool loop = true;                 // Infinite loop?
        bool playing = true;              // Is playing?
        std::string currentAnim = "idle"; // Current animation name
    };
} // namespace Ecs