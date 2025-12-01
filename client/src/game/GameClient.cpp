/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include <SFML/System/Clock.hpp>
#include "SFMLRenderer.hpp"

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

        _textureManager = std::make_shared<SFMLTextureManager>();

        _gameScene = std::make_shared<GameScene>(_renderer, _textureManager);

        _eventManager = std::make_shared<InputEventManager>();
        _inputHandler = std::make_unique<SFMLInputHandler>(_eventManager);

        _gameInputHandler = std::make_shared<Input::GameEventHandler>(_gameScene);

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

    sf::Clock clock;
    const float UPDATE_INTERVAL_MS = 16.67f;

    try {
        while (_renderer->isOpen()) {
            float frameTime = clock.getElapsedTime().asSeconds() * 1000.0f;

            if (frameTime >= UPDATE_INTERVAL_MS) {
                float deltaTime = frameTime / 1000.0f;
                clock.restart();
                _inputHandler->update(deltaTime);
                _gameScene->update(deltaTime);
            }

            sf::Event event;
            while (_renderer->pollEvent(event)) {
                if (_renderer->isWindowCloseEvent(event))
                    _renderer->close();
                _inputHandler->handleEvent(event);
            }

            _renderer->clear();
            _gameScene->render();
            _renderer->display();
        }
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected error during game loop execution: " + std::string(e.what()));
    }
}