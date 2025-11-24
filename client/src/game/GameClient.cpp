/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include "../exceptions/GameExceptions.hpp"
#include "../graphics/SFML/SFMLRenderer.hpp"
#include "../inputs/SFMLInputHandler.hpp"

void GameClient::init()
{
    try {
        renderer = std::make_unique<SFMLRenderer>();
        if (!renderer) {
            throw InitializationException("Renderer", "Failed to create renderer instance");
        }

        renderer->createWindow(800, 600, "R-Type");

        inputHandler = std::make_unique<SFMLInputHandler>();
        if (!inputHandler) {
            throw InitializationException("InputHandler", "Failed to create input handler instance");
        }
    } catch (const std::bad_alloc &e) {
        throw InitializationException("GameClient", "Memory allocation failed: " + std::string(e.what()));
    } catch (const GameException &) {
        throw; // Re-throw game exceptions as-is
    } catch (const std::exception &e) {
        throw InitializationException("GameClient", "Unexpected error: " + std::string(e.what()));
    }
}

void GameClient::run()
{
    if (!renderer || !inputHandler) {
        throw GameLoopException("Game components not properly initialized");
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
    } catch (const GameException &) {
        throw; // Re-throw game exceptions as-is
    } catch (const std::exception &e) {
        throw GameLoopException("Unexpected error during execution: " + std::string(e.what()));
    }
}