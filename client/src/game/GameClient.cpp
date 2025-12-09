/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
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

        _netClient = std::make_shared<NetClient>();
        if (!_netClient) {
            throw GameClientError("Failed to create network client instance");
        }
        _netClient->sendConnectPacket();
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
    const float UPDATE_INTERVAL_MS = 16.67f;

    try {
        while (_renderer->isOpen()) {
            float frameTime = clock.getElapsedTime().asSeconds() * 1000.0f;

            _netClient->receivePackets();

            if (frameTime >= UPDATE_INTERVAL_MS) {
                float deltaTime = frameTime / 1000.0f;
                clock.restart();
                _gameScene->update(deltaTime);
            }

            sf::Event event{sf::Event::Closed{}};
            while (_renderer->pollEvent(event)) {
                if (_renderer->isWindowCloseEvent(event) || _inputHandler->isKeyPressed(Key::Escape)) {
                    _renderer->close();
                }
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