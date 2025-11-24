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
        renderer = std::make_unique<SFMLRenderer>();
        if (!renderer) {
            throw GameClientError("Failed to create renderer instance");
        }

        renderer->createWindow(width, height, "R-Type");

        inputHandler = std::make_unique<SFMLInputHandler>();
        if (!inputHandler) {
            throw GameClientError("Failed to create input handler instance");
        }
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected initialization error: " + std::string(e.what()));
    }
}

void GameClient::run()
{
    if (!renderer || !inputHandler) {
        throw GameClientError("Game components not properly initialized");
    }

    try {
        while (renderer->isOpen()) {
            sf::Event event;
            while (renderer->pollEvent(event)) {
                if (renderer->isWindowCloseEvent(event) || inputHandler->isKeyPressed(Key::Escape)) {
                    renderer->close();
                }
                inputHandler->handleEvent(event);
            }

            renderer->clear();
            // TODO: Add rendering logic here
            renderer->display();
        }
    } catch (const std::exception &e) {
        throw GameClientError("Unexpected error during game loop execution: " + std::string(e.what()));
    }
}