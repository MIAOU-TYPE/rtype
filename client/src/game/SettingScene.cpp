/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SettingScene
*/

#include "SettingScene.hpp"
#include <iostream>

using namespace Game;
using namespace Graphics;

SettingScene::SettingScene(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ITextureManager> textureManager,
    std::shared_ptr<Audio::SFMLAudio> audioManager)
    : _renderer(std::move(renderer)), _textureManager(std::move(textureManager)), _audio(std::move(audioManager))
{
    if (!_renderer) {
        throw SettingSceneError("Renderer cannot be null");
    }

    if (!_textureManager) {
        throw SettingSceneError("Texture manager cannot be null");
    }

    try {
        const std::string bgPath = "sprites/bg-preview.png";
        if (!_textureManager->loadTexture(bgPath)) {
            throw SettingSceneError("Failed to load background texture: " + bgPath);
        }

        _backgroundSprite = _textureManager->createSprite(bgPath);
        if (!_backgroundSprite) {
            throw SettingSceneError("Failed to create background sprite");
        }

        _backgroundSprite->setPosition(-140.0f, 0.0f);
        _backgroundSprite->setScale(1.3f, 1.3f);

        unsigned int windowWidth = _renderer->getWindowWidth();
        unsigned int windowHeight = _renderer->getWindowHeight();

        float centerX = static_cast<float>(windowWidth) / 2.0f;
        float buttonSpacing = 100.0f;

        const std::string fontPath = "fonts/r-type.otf";

        Button tempButton(centerX - 100.0f, static_cast<float>(windowHeight) / 2.0f - 100.0f, "sprites/play_bt.png",
            "sprites/play_bt_hold.png", "sprites/play_bt_press.png", _textureManager, "PLAY", fontPath);

        float backButtonCenterX = centerX - (tempButton.getWidth() / 2.0f);
        initButtons(backButtonCenterX, windowHeight, fontPath);

    } catch (const std::exception &e) {
        throw SettingSceneError("Failed to initialize setting scene: " + std::string(e.what()));
    }
}

void SettingScene::initButtons(float backButtonCenterX, unsigned int windowHeight, const std::string &fontPath)
{
    _backButton = std::make_unique<Button>(backButtonCenterX - 250, static_cast<float>(windowHeight) / 2.0f + 235.0f,
        "sprites/play_bt.png", "sprites/play_bt_hold.png", "sprites/play_bt_press.png", _textureManager, "BACK",
        fontPath);
    _increaseVolumeButton = std::make_unique<Button>(backButtonCenterX + 200.0f,
        static_cast<float>(windowHeight) / 2.0f - 200.0f, "sprites/play_bt.png", "sprites/play_bt_hold.png",
        "sprites/play_bt_press.png", _textureManager, "ADD", fontPath);
    _decreaseVolumeButton = std::make_unique<Button>(backButtonCenterX - 200.0f,
        static_cast<float>(windowHeight) / 2.0f - 200.0f, "sprites/play_bt.png", "sprites/play_bt_hold.png",
        "sprites/play_bt_press.png", _textureManager, "SUB", fontPath);
    const float colorButtonScale = 3.0f;
    _protanopiaButton = std::make_unique<Button>(backButtonCenterX - 200.0f,
        static_cast<float>(windowHeight) / 2.0f - 100.0f, "sprites/play_bt.png", "sprites/play_bt_hold.png",
        "sprites/play_bt_press.png", _textureManager, "PROT", fontPath, colorButtonScale);
    _deuteranopiaButton = std::make_unique<Button>(backButtonCenterX + 31.0f,
        static_cast<float>(windowHeight) / 2.0f - 100.0f, "sprites/play_bt.png", "sprites/play_bt_hold.png",
        "sprites/play_bt_press.png", _textureManager, "DEUTER", fontPath, colorButtonScale);
    _tritanopiaButton = std::make_unique<Button>(backButtonCenterX + 265.0f,
        static_cast<float>(windowHeight) / 2.0f - 100.0f, "sprites/play_bt.png", "sprites/play_bt_hold.png",
        "sprites/play_bt_press.png", _textureManager, "TRIT", fontPath, colorButtonScale);
    setSoundButtons();
}

void SettingScene::setSoundButtons()
{
    if (_increaseVolumeButton) {
        _increaseVolumeButton->setOnClick([this]() {
            float currentVolume = _audio->getGlobalSoundVolume();
            float newVolume = std::min(currentVolume + 10.0f, 100.0f);
            _audio->setGlobalSoundVolume(newVolume);
            std::cout << "Volume increased to: " << newVolume << " from " << currentVolume << std::endl;
        });
    }

    if (_decreaseVolumeButton) {
        _decreaseVolumeButton->setOnClick([this]() {
            float currentVolume = _audio->getGlobalSoundVolume();
            float newVolume = std::max(currentVolume - 10.0f, 0.0f);
            _audio->setGlobalSoundVolume(newVolume);
            std::cout << "Volume decreased to: " << newVolume << " from " << currentVolume << std::endl;
        });
    }
}

void SettingScene::update(float mouseX, float mouseY, bool isMouseClicked)
{
    try {
        if (_backButton) {
            _backButton->update(mouseX, mouseY, isMouseClicked);
        }
        if (_increaseVolumeButton) {
            _increaseVolumeButton->update(mouseX, mouseY, isMouseClicked);
        }
        if (_decreaseVolumeButton) {
            _decreaseVolumeButton->update(mouseX, mouseY, isMouseClicked);
        }
        if (_protanopiaButton) {
            if (_protanopiaButton->update(mouseX, mouseY, isMouseClicked))
                toggleColorBlindMode(ColorBlindType::PROTANOPIA);
        }
        if (_deuteranopiaButton) {
            if (_deuteranopiaButton->update(mouseX, mouseY, isMouseClicked))
                toggleColorBlindMode(ColorBlindType::DEUTERANOPIA);
        }
        if (_tritanopiaButton) {
            if (_tritanopiaButton->update(mouseX, mouseY, isMouseClicked))
                toggleColorBlindMode(ColorBlindType::TRITANOPIA);
        }
    } catch (const std::exception &e) {
        throw SettingSceneError("Failed to update setting scene: " + std::string(e.what()));
    }
}

void SettingScene::toggleColorBlindMode(ColorBlindType type)
{
    if (!_renderer)
        return;

    ColorBlindType currentMode = _renderer->getColorBlindMode();
    if (currentMode == type) {
        _renderer->setColorBlindMode(ColorBlindType::NONE);
        return;
    }

    _renderer->setColorBlindMode(type);
}

void SettingScene::render()
{
    try {
        if (_backgroundSprite) {
            _renderer->renderSprite(*_backgroundSprite);
        }

        if (_backButton) {
            _backButton->render(_renderer);
        }

        if (_increaseVolumeButton) {
            _increaseVolumeButton->render(_renderer);
        }

        if (_decreaseVolumeButton) {
            _decreaseVolumeButton->render(_renderer);
        }

        if (_protanopiaButton)
            _protanopiaButton->render(_renderer);

        if (_deuteranopiaButton)
            _deuteranopiaButton->render(_renderer);

        if (_tritanopiaButton)
            _tritanopiaButton->render(_renderer);

        if (_errorText) {
            _renderer->renderText(*_errorText);
        }
    } catch (const std::exception &e) {
        throw SettingSceneError("Failed to render setting scene: " + std::string(e.what()));
    }
}

void SettingScene::setOnBackCallback(std::function<void()> callback)
{
    if (_backButton) {
        _backButton->setOnClick(std::move(callback));
    }
}

void SettingScene::setErrorMessage(const std::string &message)
{
    _errorMessage = message;
    if (!message.empty()) {
        const std::string fontPath = "fonts/r-type.otf";
        _errorText = _textureManager->createText(message, fontPath, 24);
        if (_errorText) {
            _errorText->setColor(255, 0, 0, 255);
            unsigned int windowWidth = _renderer->getWindowWidth();
            float textWidth = _errorText->getWidth();
            float textX = (static_cast<float>(windowWidth) - textWidth) / 2.0f;
            _errorText->setPosition(textX, 50.0f);
        }
    } else {
        _errorText.reset();
    }
}
