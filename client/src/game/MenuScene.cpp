/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** MenuScene
*/

#include "MenuScene.hpp"
#include <utility>
#include "SFMLSpriteManagement.hpp"

using namespace Game;
using namespace Graphics;

MenuScene::MenuScene(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager))
{
    if (!_renderer) {
        throw MenuSceneError("Renderer cannot be null");
    }

    if (!_textureManager) {
        throw MenuSceneError("Texture manager cannot be null");
    }

    try {
        const std::string bgPath = "client/assets/sprites/bg-preview.png";
        if (!_textureManager->loadTexture(bgPath)) {
            throw MenuSceneError("Failed to load background texture: " + bgPath);
        }

        _backgroundSprite = _textureManager->createSprite(bgPath);
        if (!_backgroundSprite) {
            throw MenuSceneError("Failed to create background sprite");
        }

        _backgroundSprite->setPosition(-140.0f, 0.0f);
        _backgroundSprite->setScale(1.3f, 1.3f);

        unsigned int windowWidth = _renderer->getWindowWidth();
        unsigned int windowHeight = _renderer->getWindowHeight();

        float centerX = static_cast<float>(windowWidth) / 2.0f;
        float buttonSpacing = 100.0f;

        const std::string fontPath = "client/assets/fonts/r-type.otf";

        _playButton = std::make_unique<Button>(centerX - 100.0f, static_cast<float>(windowHeight) / 2.0f - 100.0f,
            "client/assets/sprites/play_bt.png", "client/assets/sprites/play_bt_hold.png",
            "client/assets/sprites/play_bt_press.png", _textureManager, "PLAY", fontPath);

        float playButtonCenterX = centerX - (_playButton->getWidth() / 2.0f);
        _playButton = std::make_unique<Button>(playButtonCenterX, static_cast<float>(windowHeight) / 2.0f - 100.0f,
            "client/assets/sprites/play_bt.png", "client/assets/sprites/play_bt_hold.png",
            "client/assets/sprites/play_bt_press.png", _textureManager, "PLAY", fontPath);

        float quitButtonY = static_cast<float>(windowHeight) / 2.0f - 100.0f + _playButton->getHeight() + buttonSpacing;
        _quitButton = std::make_unique<Button>(playButtonCenterX, quitButtonY, "client/assets/sprites/play_bt.png",
            "client/assets/sprites/play_bt_hold.png", "client/assets/sprites/play_bt_press.png", _textureManager,
            "QUIT", fontPath);

    } catch (const std::exception &e) {
        throw MenuSceneError("Failed to initialize menu scene: " + std::string(e.what()));
    }
}

void MenuScene::update(float deltaTime, float mouseX, float mouseY, bool isMouseClicked)
{
    try {
        if (_playButton) {
            _playButton->update(mouseX, mouseY, isMouseClicked);
        }

        if (_quitButton) {
            _quitButton->update(mouseX, mouseY, isMouseClicked);
        }

        (void) deltaTime;

    } catch (const std::exception &e) {
        throw MenuSceneError("Failed to update menu scene: " + std::string(e.what()));
    }
}

void MenuScene::render()
{
    try {
        if (_backgroundSprite) {
            _renderer->renderSprite(*_backgroundSprite);
        }

        if (_playButton) {
            _playButton->render(_renderer);
        }

        if (_quitButton) {
            _quitButton->render(_renderer);
        }

    } catch (const std::exception &e) {
        throw MenuSceneError("Failed to render menu scene: " + std::string(e.what()));
    }
}

void MenuScene::setOnPlayCallback(std::function<void()> callback)
{
    if (_playButton) {
        _playButton->setOnClick(std::move(callback));
    }
}

void MenuScene::setOnQuitCallback(std::function<void()> callback)
{
    if (_quitButton) {
        _quitButton->setOnClick(std::move(callback));
    }
}
