/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#include "GameClient.hpp"
#include "../graphics/SFML/SFMLRenderer.hpp"

void GameClient::init() {
    renderer = std::make_unique<SFMLRenderer>();
    renderer->createWindow(800, 600, "R-Type");
}

void GameClient::run() {
    while (renderer->isOpen()) {
        sf::Event event;
        while (renderer->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                renderer->close();
            }
        }

        renderer->clear();
        // TODO: Add rendering logic here
        renderer->display();
    }
}