/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameScene
*/

#include "GameScene.hpp"

using namespace Game;

GameScene::GameScene(
    std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager))
{
    if (!_renderer) {
        throw GameSceneError("Renderer cannot be null");
    }

    if (!_textureManager) {
        throw GameSceneError("Texture manager cannot be null");
    }

    try {
        _starfield = std::make_unique<Background::Starfield>(_renderer, _textureManager);
        if (!_starfield)
            throw GameSceneError("Failed to create starfield instance");
        _entityDrawing = std::make_shared<Graphics::SFMLEntityDrawing>(_renderer, _textureManager);
        _inputSystem = std::make_unique<Ecs::InputSystem>(_registry);
        if (!_inputSystem)
            throw GameSceneError("Failed to create input system instance");
        _gameWorld = std::make_shared<GameWorld>(_registry);
        if (!_gameWorld)
            throw GameSceneError("Failed to create game world instance");
        _audioResourceManager = std::make_shared<Resources::EmbeddedResourceManager>();
        if (!_audioResourceManager)
            throw GameSceneError("Failed to create audio resource manager instance");
        _audioManager = std::make_unique<Audio::SFMLAudio>(_audioResourceManager);
        if (!_audioManager)
            throw GameSceneError("Failed to create audio manager instance");
        _audioManager->playMusic("game_theme", true);
    } catch (const std::exception &e) {
        throw GameSceneError("Failed to initialize game scene: " + std::string(e.what()));
    }
}

void GameScene::update(float deltaTime)
{
    _gameWorld->update(deltaTime);
    _inputSystem->update(deltaTime);
    _starfield->update(deltaTime);
    _registry.view<Ecs::Position, Ecs::SpriteTag>([&](Ecs::Entity e, auto &pos, auto &sprite) {
        auto &linkOpt = _registry.getComponents<Ecs::RenderLink>()[static_cast<size_t>(e)];

        if (!linkOpt) {
            auto gfx = _entityDrawing->createEntity(pos.x, pos.y, sprite.name);
            _registry.emplaceComponent<Ecs::RenderLink>(e, Ecs::RenderLink{gfx});
        } else {
            linkOpt->gfx->setPosition(pos.x, pos.y);
        }
    });
    _entityDrawing->updateAllEntities(deltaTime);
}

void GameScene::render() const
{
    _starfield->render();
    _entityDrawing->renderAllEntities();
}

Ecs::Registry &GameScene::getRegistry()
{
    return _registry;
}

GameWorld &GameScene::getGameWorld() const
{
    return *_gameWorld;
}

std::shared_ptr<GameWorld> GameScene::getGameWorldPtr()
{
    return _gameWorld;
}
