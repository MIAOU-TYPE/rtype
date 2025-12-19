/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"

namespace Engine
{
    void RenderSystem::submit(const ClientEntity &e, const Animation &anim, Graphics::IRenderer &renderer)
    {
        RenderCommand cmd;
        cmd.textureId = e.render.texture;
        cmd.frame = anim.frames[e.animation.frameIndex].rect;
        cmd.position = e.position;

        renderer.draw(cmd);
    }
} // namespace Engine