/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteRegistry
*/

#pragma once

#include "SpriteRegistry.hpp"

namespace Engine
{
    template <typename Fn>
    void SpriteRegistry::forEach(Fn &&fn)
    {
        for (auto &[id, def] : _sprites)
            fn(id, def);
    }
} // namespace Engine