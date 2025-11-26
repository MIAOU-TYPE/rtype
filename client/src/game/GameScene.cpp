/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameScene
*/

#include "GameScene.hpp"

#include <utility>

#include <utility>

using namespace Game;

GameScene::GameScene(
    std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager)
    : _renderer(std::move(std::move(renderer))), _textureManager(std::move(std::move(textureManager)))
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

        // TODO: Render other game entities here

    } catch (const std::exception &e) {
        throw GameSceneError("Failed to render game scene: " + std::string(e.what()));
    }
}
