/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EntityClient
*/

#pragma once
#include <string>
#include "Animation/AnimationState.hpp"
#include "Position.hpp"
#include "RenderComponent.hpp"

namespace Engine
{
    /**
     * @struct ClientEntity
     * @brief Represents a client-side entity with position, sprite, animation, and rendering components
     * for rendering in the game.
     */
    struct ClientEntity {
        size_t id;                ///> Unique identifier for the entity
        Vec2<float> position;     ///> Position of the entity in 2D space
        std::string spriteId;     ///> Identifier for the sprite associated with the entity
        AnimationState animation; ///> Animation state of the entity
        RenderComponent render;   ///> Rendering component of the entity
    };
} // namespace Engine