/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"

namespace Engine
{
    void RenderSystem::submit(const Position &pos, const Render &render, const AnimationState &animState,
        const Animation &anim, Graphics::IRenderer &renderer)
    {
        RenderCommand cmd;
        cmd.textureId = render.texture;
        cmd.frame = anim.frames[animState.frameIndex].rect;
        cmd.position = {pos.x, pos.y};
        renderer.draw(cmd);
    }
} // namespace Engine