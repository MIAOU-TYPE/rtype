/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameOverScene
*/

#include "GameOverScene.hpp"

using namespace Game;
using namespace Graphics;

GameOverScene::GameOverScene(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager))
{
    if (!_renderer) {
        throw GameOverSceneError("Renderer cannot be null");
    }

    if (!_textureManager) {
        throw GameOverSceneError("Texture manager cannot be null");
    }

    try {
        const std::string bgPath = "sprites/game_over.png";
        if (!_textureManager->loadTexture(bgPath)) {
            throw GameOverSceneError("Failed to load background texture: " + bgPath);
        }

        _backgroundSprite = _textureManager->createSprite(bgPath);
        if (!_backgroundSprite) {
            throw GameOverSceneError("Failed to create background sprite");
        }

        _backgroundSprite->setPosition(0.0f, 0.0f);
        _backgroundSprite->setScale(3.1f, 3.1f);

        unsigned int windowWidth = _renderer->getWindowWidth();
        unsigned int windowHeight = _renderer->getWindowHeight();

        float centerX = static_cast<float>(windowWidth) / 2.0f;

        const std::string fontPath = "fonts/r-type.otf";

        Button tempButton(centerX - 100.0f, static_cast<float>(windowHeight) / 2.0f, "sprites/play_bt.png",
            "sprites/play_bt_hold.png", "sprites/play_bt_press.png", _textureManager, "PLAY AGAIN", fontPath, 20);

        float buttonCenterX = centerX - (tempButton.getWidth() / 2.0f);
        _returnToMenuButton = std::make_unique<Button>(buttonCenterX, static_cast<float>(windowHeight) / 2.0f + 150.0f,
            "sprites/play_bt.png", "sprites/play_bt_hold.png", "sprites/play_bt_press.png", _textureManager,
            "PLAY AGAIN", fontPath, 20);

    } catch (const std::exception &e) {
        throw GameOverSceneError("Failed to initialize game over scene: " + std::string(e.what()));
    }
}

void GameOverScene::update(float mouseX, float mouseY, bool isMouseClicked)
{
    try {
        _returnToMenuButton->update(mouseX, mouseY, isMouseClicked);
    } catch (const std::exception &e) {
        throw GameOverSceneError("Failed to update game over scene: " + std::string(e.what()));
    }
}

void GameOverScene::render()
{
    try {
        _renderer->renderSprite(*_backgroundSprite);
        _returnToMenuButton->render(_renderer);
    } catch (const std::exception &e) {
        throw GameOverSceneError("Failed to render game over scene: " + std::string(e.what()));
    }
}

void GameOverScene::setOnReturnToMenuCallback(std::function<void()> callback)
{
    if (_returnToMenuButton) {
        _returnToMenuButton->setOnClick(std::move(callback));
    }
}
