/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ClientWorld
*/

#include "ClientWorld.hpp"

namespace Engine
{
    ClientWorld::ClientWorld(std::shared_ptr<SpriteRegistry> spriteRegistry)
        : _spriteRegistry(std::move(spriteRegistry))
    {
        _registry.registerComponent<Position>();
        _registry.registerComponent<Drawable>();
        _registry.registerComponent<AnimationState>();
        _registry.registerComponent<Render>();
    }

    Ecs::Registry &ClientWorld::registry()
    {
        return _registry;
    }

    void ClientWorld::update(float dt, Graphics::IRenderer &renderer)
    {
        _registry.view<Position, Drawable, AnimationState, Render>(
            [&](Ecs::Entity, Position &pos, Drawable &drawable, AnimationState &animState, Render &render) {
                const SpriteDefinition &sprite = _spriteRegistry->get(drawable.spriteId);

                if (animState.currentAnimation.empty())
                    animState.currentAnimation = sprite.defaultAnimation;

                const auto animIt = sprite.animations.find(animState.currentAnimation);
                if (animIt == sprite.animations.end())
                    return;

                const Animation &anim = animIt->second;

                AnimationSystem::update(animState, anim, dt);
                RenderSystem::submit(pos, render, animState, anim, renderer);
            });
    }

} // namespace Engine
