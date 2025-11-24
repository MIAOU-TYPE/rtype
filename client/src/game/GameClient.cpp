/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include "../graphics/SFML/SFMLRenderer.hpp"
#include "../inputs/SFMLInputHandler.hpp"

void GameClient::init() {
    renderer = std::make_unique<SFMLRenderer>();
    renderer->createWindow(800, 600, "R-Type");
    inputHandler = std::make_unique<SFMLInputHandler>();
}

void GameClient::run() {
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
}