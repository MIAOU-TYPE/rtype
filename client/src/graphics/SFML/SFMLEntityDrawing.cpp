/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityDrawing
*/

#include "SFMLEntityDrawing.hpp"
#include <utility>
#include <utility>
#include <vector>

using namespace Graphics;

std::unordered_map<std::string, SFMLEntityDrawing::SpriteInfo> SFMLEntityDrawing::_spriteInfo = {
    {"player", {.path="client/assets/sprites/player.png", .width=33.1f, .height=18.0f}},
    {"enemie", {.path="client/assets/sprites/enemie.png", .width=65.1f, .height=66.0f}}};

SFMLEntityDrawing::SFMLEntityDrawing(
    std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager)
    : _renderer(std::move(std::move(renderer))), _textureManager(std::move(std::move(textureManager)))
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

    return {.path="client/assets/sprites/" + spriteName + ".png", .width=32.0f, .height=32.0f};
}

std::string SFMLEntityDrawing::getSpritePathFromName(const std::string &spriteName)
{
    return getSpriteInfoFromName(spriteName).path;
}
