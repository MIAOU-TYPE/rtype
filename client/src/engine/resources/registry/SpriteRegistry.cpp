/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteRegistry
*/

#include "SpriteRegistry.hpp"

namespace Engine
{
    void SpriteRegistry::add(const unsigned int spriteId, SpriteDefinition def)
    {
        _sprites.emplace(spriteId, std::move(def));
    }

    const SpriteDefinition &SpriteRegistry::get(const unsigned int spriteId) const
    {
        const auto it = _sprites.find(spriteId);
        if (it == _sprites.end())
            throw SpriteRegistryError("{SpriteRegistry::get}: unknown spriteId: " + std::to_string(spriteId));
        return it->second;
    }

    bool SpriteRegistry::exists(const unsigned int spriteId) const
    {
        return _sprites.contains(spriteId);
    }

} // namespace Engine
