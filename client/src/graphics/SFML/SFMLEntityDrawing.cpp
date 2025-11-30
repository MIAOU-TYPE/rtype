/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityDrawing
*/

#include "SFMLEntityDrawing.hpp"
#include "SFMLEntityCreation.hpp"
#include <utility>
#include <vector>

using namespace Graphics;

SFMLEntityDrawing::SFMLEntityDrawing(
    std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager))
{
    if (!_renderer) {
        throw SFMLEntityDrawingError("Renderer cannot be null");
    }
    if (!_textureManager) {
        throw SFMLEntityDrawingError("Texture manager cannot be null");
    }

    _spriteInfo = {
        {"player", {"client/assets/sprites/player.png", 33.1f, 18.0f}},
        {"enemy", {"client/assets/sprites/enemy.png", 65.1f, 66.0f}}
    };
}

std::shared_ptr<GraphicalEntity> SFMLEntityDrawing::createEntity(float x, float y, const std::string &spriteName)
{
    try {
        auto entity = std::make_shared<GraphicalEntity>(x, y, spriteName, _textureManager, *this);
        _entities.push_back(entity);
        return entity;
    } catch (const std::exception &e) {
        throw SFMLEntityDrawingError("Failed to create entity '" + spriteName + "': " + std::string(e.what()));
    }
}

void SFMLEntityDrawing::renderAllEntities()
{
    for (auto &entity : _entities) {
        if (entity) {
            entity->render(_renderer);
        }
    }
}

SpriteInfo SFMLEntityDrawing::getSpriteInfoFromName(const std::string &spriteName) const
{
    auto it = _spriteInfo.find(spriteName);
    if (it != _spriteInfo.end()) {
        return it->second;
    }

    throw SFMLEntityDrawingError(
        "Unknown sprite name: '" + spriteName + "'. Please add it to _spriteInfo map with proper dimensions.");
}

std::string SFMLEntityDrawing::getSpritePathFromName(const std::string &spriteName) const
{
    return getSpriteInfoFromName(spriteName).path;
}
