/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameScene
*/

#include "GameScene.hpp"
#include "InputSystem.hpp"

#include <utility>

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
        if (!_starfield) {
            throw GameSceneError("Failed to create starfield instance");
        }

        // _entityDrawing = std::make_unique<Graphics::SFMLEntityDrawing>(_renderer, _textureManager);
        // if (!_entityDrawing) {
        //     throw GameSceneError("Failed to create entity drawing instance");
        // }

        // _entityDrawing->createEntity(100.0f, 300.0f, "player", 0);
        // _entityDrawing->createEntity(400.0f, 200.0f, "enemy", 1);
        // _entityDrawing->createEntity(500.0f, 400.0f, "enemy", 2);
        // _entityDrawing->createEntity(600.0f, 300.0f, "missile", 3);
        // _entityDrawing->createEntity(700.0f, 350.0f, "explose", 4);

    } catch (const std::exception &e) {
        throw GameSceneError("Failed to initialize game scene: " + std::string(e.what()));
    }
}

void GameScene::update(float deltaTime)
{
    try {
        if (_starfield) {
            _starfield->update(deltaTime);
        }

        Ecs::InputSystem::update(_registry, deltaTime);

        // if (_entityDrawing) {
        //     _entityDrawing->updateAllEntities(deltaTime);
        // }

        // TODO: Update other game entities here

    } catch (const std::exception &e) {
        throw GameSceneError("Failed to update game scene: " + std::string(e.what()));
    }
}

void GameScene::render()
{
    try {
        if (_starfield) {
            _starfield->render();
        }

        // if (_entityDrawing) {
        //     _entityDrawing->renderAllEntities();
        // }

    } catch (const std::exception &e) {
        throw GameSceneError("Failed to render game scene: " + std::string(e.what()));
    }
}

Ecs::Registry &GameScene::getRegistry()
{
    return _registry;
}
