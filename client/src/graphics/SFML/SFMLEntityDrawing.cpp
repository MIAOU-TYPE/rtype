/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityDrawing
*/

#include "SFMLEntityDrawing.hpp"
#include "SFMLEntityCreation.hpp"

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

    std::vector<AnimationFrame> playerIdleFrames = {AnimationFrame{0, 0, 33, 18, 0.15f},
        AnimationFrame{33, 0, 33, 18, 0.15f}, AnimationFrame{66, 0, 33, 18, 0.15f},
        AnimationFrame{99, 0, 33, 18, 0.15f}, AnimationFrame{132, 0, 33, 18, 0.15f},
        AnimationFrame{165, 0, 33, 18, 0.15f}, AnimationFrame{198, 0, 33, 18, 0.15f},
        AnimationFrame{231, 0, 33, 18, 0.15f}};

    auto playerIdleAnim = std::make_shared<SFMLAnimation>("idle", playerIdleFrames, true);

    std::vector<AnimationFrame> enemyIdleFrames = {AnimationFrame{0, 0, 65, 66, 1.f},
        AnimationFrame{65, 0, 65, 66, 0.3f}, AnimationFrame{130, 0, 65, 66, 0.3f}, AnimationFrame{195, 0, 65, 66, 0.3f},
        AnimationFrame{260, 0, 65, 66, 0.3f}, AnimationFrame{325, 0, 65, 66, 0.3f},
        AnimationFrame{390, 0, 65, 66, 0.3f}, AnimationFrame{455, 0, 65, 66, 0.3f}};

    auto enemyIdleAnim = std::make_shared<SFMLAnimation>("idle", enemyIdleFrames, true);

    std::vector<AnimationFrame> missileFlyFrames = {
        AnimationFrame{0, 0, 16, 12, 0.1f}, AnimationFrame{16, 0, 16, 12, 0.3f}};

    auto missileFlyAnim = std::make_shared<SFMLAnimation>("fly", missileFlyFrames, false);

    std::vector<AnimationFrame> entityExploseFrames = {AnimationFrame{0, 0, 33, 32, 0.1f},
        AnimationFrame{33, 0, 33, 32, 0.1f}, AnimationFrame{66, 0, 33, 32, 0.1f}, AnimationFrame{99, 0, 33, 32, 0.1f},
        AnimationFrame{132, 0, 33, 32, 0.1f}, AnimationFrame{165, 0, 33, 32, 0.1f},
        AnimationFrame{198, 0, 33, 32, 0.1f}};

    auto entityExploseAnim = std::make_shared<SFMLAnimation>("explose", entityExploseFrames, false);

    _spriteInfo = {{"player", {"sprites/player.png", 33.1f, 18.0f, {AnimationInfo{playerIdleAnim}}, "idle"}},
        {"player2", {"sprites/player2.png", 33.1f, 18.0f, {AnimationInfo{playerIdleAnim}}, "idle"}},
        {"player3", {"sprites/player3.png", 33.1f, 18.0f, {AnimationInfo{playerIdleAnim}}, "idle"}},
        {"player4", {"sprites/player4.png", 33.1f, 18.0f, {AnimationInfo{playerIdleAnim}}, "idle"}},
        {"enemy", {"sprites/enemy.png", 65.1f, 66.0f, {AnimationInfo{enemyIdleAnim}}, "idle"}},
        {"missile", {"sprites/missile.png", 16.5f, 12.0f, {AnimationInfo{missileFlyAnim}}, "fly"}},
        {"explose", {"sprites/explose.png", 33.0f, 32.0f, {AnimationInfo{entityExploseAnim}}, "explose"}}};
}

std::shared_ptr<GraphicalEntity> SFMLEntityDrawing::createEntity(
    float x, float y, const std::string &spriteName, size_t id)
{
    try {
        size_t actualId = (id == 0) ? _nextEntityId++ : id;
        auto entity = std::make_shared<GraphicalEntity>(x, y, spriteName, _textureManager, *this, actualId);
        _entities.push_back(entity);
        return entity;
    } catch (const std::exception &e) {
        throw SFMLEntityDrawingError("Failed to create entity '" + spriteName + "': " + std::string(e.what()));
    }
}

void SFMLEntityDrawing::updateAllEntities(float deltaTime)
{
    for (auto &entity : _entities) {
        if (entity) {
            entity->update(deltaTime);
        }
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

std::shared_ptr<SFMLAnimationManager> SFMLEntityDrawing::createAnimationManager(const std::string &spriteName) const
{
    const SpriteInfo &spriteInfo = getSpriteInfoFromName(spriteName);

    auto animationManager = std::make_shared<SFMLAnimationManager>();

    for (const auto &animInfo : spriteInfo.animations) {
        if (animInfo.animation) {
            animationManager->addAnimation(animInfo.animation);
        }
    }

    if (!spriteInfo.defaultAnimation.empty() && animationManager->hasAnimation(spriteInfo.defaultAnimation)) {
        animationManager->setCurrentAnimation(spriteInfo.defaultAnimation);
    } else if (!spriteInfo.animations.empty() && spriteInfo.animations[0].animation) {
        animationManager->setCurrentAnimation(spriteInfo.animations[0].animation->getName());
    } else {
        throw SFMLEntityDrawingError("No animations available for sprite '" + spriteName + "'");
    }

    return animationManager;
}

std::shared_ptr<GraphicalEntity> SFMLEntityDrawing::getEntity(size_t index) const
{
    if (index >= _entities.size()) {
        return nullptr;
    }
    return _entities[index];
}

size_t SFMLEntityDrawing::getEntityCount() const
{
    return _entities.size();
}

bool SFMLEntityDrawing::shouldAnimationLoop(const std::string &spriteName, const std::string &animationName) const
{
    const SpriteInfo &spriteInfo = getSpriteInfoFromName(spriteName);

    for (const auto &animInfo : spriteInfo.animations) {
        if (animInfo.animation && animInfo.animation->getName() == animationName) {
            return animInfo.animation->getLoop();
        }
    }

    throw SFMLEntityDrawingError("Animation '" + animationName + "' not found for sprite '" + spriteName + "'");
}

std::shared_ptr<GraphicalEntity> SFMLEntityDrawing::getEntityById(size_t id) const
{
    for (const auto &entity : _entities) {
        if (entity && entity->getId() == id) {
            return entity;
        }
    }
    return nullptr;
}

bool SFMLEntityDrawing::removeEntityById(size_t id)
{
    auto it = std::find_if(_entities.begin(), _entities.end(), [id](const std::shared_ptr<GraphicalEntity> &entity) {
        return entity && entity->getId() == id;
    });

    if (it != _entities.end()) {
        _entities.erase(it);
        return true;
    }
    return false;
}

bool SFMLEntityDrawing::removeEntity(const std::shared_ptr<GraphicalEntity> &entity)
{
    auto it = std::find(_entities.begin(), _entities.end(), entity);
    if (it != _entities.end()) {
        _entities.erase(it);
        return true;
    }
    return false;
}

void SFMLEntityDrawing::clearAllEntities()
{
    _entities.clear();
    _nextEntityId = 0;
}
