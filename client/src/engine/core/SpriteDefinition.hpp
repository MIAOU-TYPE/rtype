/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteDefinition
*/

#pragma once
#include <string>
#include "Animation.hpp"
#include "ITextureManager.hpp"
#include "IAudioManager.hpp"
#include <unordered_map>
#include <optional>

namespace Engine
{
    /**
     * @struct SpriteDefinition
     * @brief Defines a sprite with its texture path, default animation, and available animations.
     */
    struct SpriteDefinition {
        std::string texturePath;                                          ///> Path to the texture file
        Graphics::TextureHandle textureHandle = Graphics::InvalidTexture; ///> Handle to the loaded texture
        std::string defaultAnimation;                                     ///> Name of the default animation
        std::unordered_map<std::string, Animation> animations; ///> Map of animation names to Animation objects
        
        std::optional<std::string> shootSoundPath;   ///> Path to shoot sound effect
        std::optional<std::string> hitSoundPath;     ///> Path to hit/damage sound effect
        std::optional<std::string> destroySoundPath; ///> Path to destruction/explosion sound effect
        
        Graphics::AudioHandle shootSoundHandle = Graphics::InvalidAudio;   ///> Handle to loaded shoot sound
        Graphics::AudioHandle hitSoundHandle = Graphics::InvalidAudio;     ///> Handle to loaded hit sound
        Graphics::AudioHandle destroySoundHandle = Graphics::InvalidAudio; ///> Handle to loaded destroy sound
    };
} // namespace Engine