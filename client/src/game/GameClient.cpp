/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include <SFML/System/Clock.hpp>
#include "SFMLInputHandler.hpp"
#include "SFMLRenderer.hpp"

using namespace Graphics;
using namespace Input;
using namespace Game;

void GameClient::init(unsigned int width, unsigned int height)
{
    try {
        _renderer = std::make_shared<SFMLRenderer>();
        if (!_renderer) {
            throw GameClientError("Failed to create renderer instance");
        }

        _renderer->createWindow(width, height, "R-Type");

        _inputHandler = std::make_unique<SFMLInputHandler>();

        _textureManager = std::make_shared<SFMLTextureManager>();
        if (!_textureManager) {
            throw GameClientError("Failed to create texture manager instance");
        }

        _gameScene = std::make_unique<GameScene>(_renderer, _textureManager);
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected initialization error: " + std::string(e.what()));
    }
}

void GameClient::run()
{
    if (!_renderer || !_inputHandler || !_gameScene) {
        throw GameClientError("Game components not properly initialized");
    }

    sf::Clock clock;

    try {
        while (_renderer->isOpen()) {
            float deltaTime = clock.restart().asSeconds();

            sf::Event event;
            while (_renderer->pollEvent(event)) {
                if (_renderer->isWindowCloseEvent(event) || _inputHandler->isKeyPressed(Key::Escape)) {
                    _renderer->close();
                }
                _inputHandler->handleEvent(event);
            }

            _gameScene->update(deltaTime);

            _renderer->clear();

            _gameScene->render();

            _renderer->display();
        }
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected error during game loop execution: " + std::string(e.what()));
    }
}