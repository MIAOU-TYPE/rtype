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

void GameScene::update(float deltaTime) const
{
    _gameWorld->update(deltaTime);
    _inputSystem->update(deltaTime);
    _starfield->update(deltaTime);
}

void GameScene::render() const
{
    try {
        _starfield->render();
    } catch (const std::exception &e) {
        throw GameSceneError("Failed to update game scene: " + std::string(e.what()));
    }
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
