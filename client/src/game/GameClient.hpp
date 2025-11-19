/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <memory>
#include <SFML/Window/Event.hpp>
#include "../graphics/interfaces/IRenderer.hpp"
#include "../inputs/interfaces/IInputHandler.hpp"

/**
 * @class GameClient
 * @brief Main client class responsible for initializing and running the game.
 *
 * The GameClient manages the renderer, handles the game loop, and coordinates
 * between different subsystems like rendering, networking, and ECS.
 */
class GameClient {
private:
    std::unique_ptr<IRenderer> renderer; ///< The renderer interface
    std::unique_ptr<IInputHandler> inputHandler; ///< The input handler interface

public:
    /**
     * @brief Initializes the game client.
     *
     * Sets up the renderer and other necessary components.
     */
    void init();

    /**
     * @brief Runs the main game loop.
     *
     * Handles events, updates game state, and renders the scene.
     */
    void run();
};

#endif // GAME_CLIENT_HPP