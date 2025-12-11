/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include <SFML/Window/Event.hpp>
#include <optional>
#include "InputComponent.hpp"
#include "Position.hpp"
#include "SFMLInputHandler.hpp"
#include "SFMLRenderer.hpp"
#include "ShootingComponent.hpp"
#include "Velocity.hpp"
#include "resources/EmbeddedResourceManager.hpp"

using namespace Graphics;
using namespace Input;
using namespace Game;
using namespace Events;

void GameClient::init(unsigned int width, unsigned int height)
{
    try {
        _renderer = std::make_shared<SFMLRenderer>();
        if (!_renderer) {
            throw GameClientError("Failed to create renderer instance");
        }
        _renderer->createWindow(width, height, "R-Type");

        auto resourceManager = std::make_shared<Resources::EmbeddedResourceManager>();
        _textureManager = std::make_shared<SFMLTextureManager>(resourceManager);

        _netClient = std::make_shared<NetClient>();
        if (!_netClient) {
            throw GameClientError("Failed to create network client instance");
        }

        _gameScene = std::make_shared<GameScene>(_renderer, _textureManager);
        _menuScene = std::make_shared<MenuScene>(_renderer, _textureManager);

        _menuScene->setOnPlayCallback([this]() {
            _currentScene = SceneState::Gameplay;
            _isMousePressed = false;
        });

        _menuScene->setOnQuitCallback([this]() {
            if (_netClient)
                _netClient->close();
            _renderer->close();
        });

        auto &registry = _gameScene->getRegistry();

        registry.registerComponent<Ecs::InputComponent>();
        registry.registerComponent<Ecs::Velocity>();
        registry.registerComponent<Ecs::Position>();
        registry.registerComponent<Ecs::ShootingComponent>();

        _playerEntity = registry.createEntity();
        registry.emplaceComponent<Ecs::InputComponent>(_playerEntity);
        registry.emplaceComponent<Ecs::Velocity>(_playerEntity, 0.f, 0.f);
        registry.emplaceComponent<Ecs::Position>(_playerEntity, 100.f, 300.f);
        registry.emplaceComponent<Ecs::ShootingComponent>(_playerEntity);

        _eventManager = std::make_shared<InputEventManager>();
        _inputHandler = std::make_shared<SFMLInputHandler>(_eventManager);

        _gameInputHandler = std::make_shared<Input::GameEventHandler>(_gameScene);
        _gameInputHandler->setPlayerEntity(_playerEntity);

        _gameInputHandler->setQuitCallback([this]() {
            _renderer->close();
        });

        std::vector<InputAction> actions = {InputAction::MoveUp, InputAction::MoveDown, InputAction::MoveLeft,
            InputAction::MoveRight, InputAction::Shoot, InputAction::Pause, InputAction::Quit, InputAction::Confirm,
            InputAction::Cancel};

        for (const auto &action : actions)
            _eventManager->registerHandler(action, _gameInputHandler);

    } catch (const std::exception &e) {
        throw GameClientError("Unexpected initialization error: " + std::string(e.what()));
    }
}

void GameClient::run()
{
    if (!_renderer || !_inputHandler || !_gameScene || !_eventManager) {
        throw GameClientError("Game components not properly initialized");
    }

    const float UPDATE_INTERVAL_MS = 16.67f;

    try {
        while (_renderer->isOpen()) {
            float frameTime = _renderer->getElapsedTime() * 1000.0f;

            if (frameTime >= UPDATE_INTERVAL_MS) {
                float deltaTime = frameTime / 1000.0f;
                _renderer->restartClock();

                if (_netClient)
                    _netClient->updatePing(deltaTime);

                if (_currentScene == SceneState::Menu) {
                    float mouseX = 0.0f;
                    float mouseY = 0.0f;
                    _renderer->getMousePosition(mouseX, mouseY);
                    _menuScene->update(mouseX, mouseY, _isMousePressed);
                } else {
                    _inputHandler->update(deltaTime);
                    _gameScene->update(deltaTime);
                }
            }

            std::shared_ptr<Graphics::IEvent> event;
            while (_renderer->pollEvent(event)) {
                if (_renderer->isWindowCloseEvent(*event)) {
                    if (_netClient)
                        _netClient->close();
                    _renderer->close();
                }

                if (_currentScene == SceneState::Menu) {
                    if (event->isMouseButtonPressed()) {
                        _isMousePressed = true;
                    } else if (event->isMouseButtonReleased()) {
                        _isMousePressed = false;
                    }
                } else {
                    _inputHandler->handleEvent(event);
                }
            }

            _renderer->clear();

            if (_currentScene == SceneState::Menu) {
                _menuScene->render();
            } else {
                _gameScene->render();
            }

            _renderer->display();
        }
    } catch (const std::exception &e) {
        if (_netClient)
            _netClient->close();
        throw GameClientError("Unexpected error during game loop execution: " + std::string(e.what()));
    }
}
