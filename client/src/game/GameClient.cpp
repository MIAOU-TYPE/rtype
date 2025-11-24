/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include "SFMLInputHandler.hpp"
#include "SFMLRenderer.hpp"

using namespace Graphics;
using namespace Input;
using namespace Game;

void GameClient::init(unsigned int width, unsigned int height)
{
    try {
        _renderer = std::make_unique<SFMLRenderer>();
        if (!_renderer) {
            throw GameClientError("Failed to create renderer instance");
        }

        _renderer->createWindow(width, height, "R-Type");

        _inputHandler = std::make_unique<SFMLInputHandler>();
        if (!_inputHandler) {
            throw GameClientError("Failed to create input handler instance");
        }
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected initialization error: " + std::string(e.what()));
    }
}

void GameClient::run()
{
    if (!_renderer || !_inputHandler) {
        throw GameClientError("Game components not properly initialized");
    }

    try {
        while (_renderer->isOpen()) {
            sf::Event event;
            while (_renderer->pollEvent(event)) {
                if (_renderer->isWindowCloseEvent(event) || _inputHandler->isKeyPressed(Key::Escape)) {
                    _renderer->close();
                }
                _inputHandler->handleEvent(event);
            }

            _renderer->clear();
            // TODO: Add rendering logic here
            _renderer->display();
        }
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected error during game loop execution: " + std::string(e.what()));
    }
}