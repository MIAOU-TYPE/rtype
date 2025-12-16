/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** DisplayInit
*/

#include "DisplayInit.hpp"

using namespace Display;

DisplayInit::DisplayInit(std::shared_ptr<Graphics::IRenderer> renderer) : _renderer(renderer)
{
    if (!_renderer) {
        throw DisplayInitError("Renderer is null");
    }
    _audioResourceManager = std::make_shared<Resources::EmbeddedResourceManager>();
    if (!_audioResourceManager) {
        throw DisplayInitError("Failed to create audio resource manager instance");
    }

    _audioManager = std::make_unique<Audio::SFMLAudio>(_audioResourceManager);
    if (!_audioManager) {
        throw DisplayInitError("Failed to create audio manager instance");
    }

    _audioManager->playMusic("menu_theme", true);
}

void DisplayInit::run()
{
    if (!_renderer || !_gameScene) {
        throw DisplayInitError("Game components not properly initialized");
    }

    try {
        float frameTime = _renderer->getElapsedTime() * 1000.0f;
        if (frameTime >= UPDATE_INTERVAL_MS) {
            float deltaTime = frameTime / 1000.0f;
            _renderer->restartClock();
            if (_currentScene == SceneState::Menu) {
                float mouseX = 0.0f;
                float mouseY = 0.0f;
                _renderer->getMousePosition(mouseX, mouseY);
                _menuScene->update(mouseX, mouseY, _renderer->getIsMousePressed());
            } else if (_currentScene == SceneState::Setting) {
                float mouseX = 0.0f;
                float mouseY = 0.0f;
                _renderer->getMousePosition(mouseX, mouseY);
                _settingScene->update(mouseX, mouseY, _renderer->getIsMousePressed());
            } else {
                _gameScene->update(deltaTime);
            }
        }

        _renderer->clear();

        if (_currentScene == SceneState::Menu) {
            _menuScene->render();
        } else if (_currentScene == SceneState::Setting) {
            _settingScene->render();
        } else {
            _gameScene->render();
        }

        _renderer->display();
    } catch (const std::exception &e) {
        throw DisplayInitError("Unexpected error during game loop execution: " + std::string(e.what()));
    }
}

void DisplayInit::init(unsigned int width, unsigned int height)
{
    if (!_renderer) {
        throw DisplayInitError("Renderer not initialized");
    }

    try {
        _renderer->createWindow(width, height, "R-Type Client");

        auto resourceManager = std::make_shared<Resources::EmbeddedResourceManager>();
        _textureManager = std::make_shared<Graphics::SFMLTextureManager>(resourceManager);
        _gameScene = std::make_shared<Game::GameScene>(_renderer, _textureManager, _audioManager);
        _menuScene = std::make_shared<Game::MenuScene>(_renderer, _textureManager);
        _settingScene = std::make_shared<Game::SettingScene>(_renderer, _textureManager, _audioManager);

        _menuScene->setOnPlayCallback([this]() {
            _currentScene = SceneState::Gameplay;
            _renderer->setIsMousePressed(false);
            _audioManager->playMusic("game_theme", true);
            _audioManager->setGlobalSoundVolume(_audioManager->getGlobalSoundVolume());
        });

        _menuScene->setOnQuitCallback([this]() {
            _renderer->close();
        });

        _menuScene->setOnSettingsCallback([this]() {
            _currentScene = SceneState::Setting;
        });

        _settingScene->setOnBackCallback([this]() {
            _currentScene = SceneState::Menu;
        });
    } catch (const std::exception &e) {
        throw DisplayInitError("Failed to create window: " + std::string(e.what()));
    }
}

bool DisplayInit::isWindowOpen() const
{
    return _renderer && _renderer->isOpen();
}

std::shared_ptr<Graphics::IRenderer> DisplayInit::getRenderer() const
{
    return _renderer;
}
