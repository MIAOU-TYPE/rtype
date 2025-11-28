/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityDrawing
*/

#include "SFMLEntityDrawing.hpp"
#include <utility>
#include <vector>

using namespace Graphics;

std::unordered_map<std::string, SFMLEntityDrawing::SpriteInfo> SFMLEntityDrawing::_spriteInfo = {
    {"player", {"client/assets/sprites/player.png", 33.1f, 18.0f}},
    {"enemy", {"client/assets/sprites/enemy.png", 65.1f, 66.0f}}};

SFMLEntityDrawing::SFMLEntityDrawing(
    std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager))
{
    if (!_renderer) {
        throw std::runtime_error("Renderer cannot be null");
    }
    if (!_textureManager) {
        throw std::runtime_error("Texture manager cannot be null");
    }
}

std::shared_ptr<GraphicalEntity> SFMLEntityDrawing::createEntity(float x, float y, const std::string &spriteName)
{
    try {
        auto entity = std::make_shared<GraphicalEntity>(x, y, spriteName, _textureManager);
        _entities.push_back(entity);
        return entity;
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to create entity '" + spriteName + "': " + std::string(e.what()));
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

SFMLEntityDrawing::SpriteInfo SFMLEntityDrawing::getSpriteInfoFromName(const std::string &spriteName)
{
    auto it = _spriteInfo.find(spriteName);
    if (it != _spriteInfo.end()) {
        return it->second;
    }

    return {"client/assets/sprites/" + spriteName + ".png", 32.0f, 32.0f};
}

std::string SFMLEntityDrawing::getSpritePathFromName(const std::string &spriteName)
{
    return getSpriteInfoFromName(spriteName).path;
}
