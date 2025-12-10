/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameClient
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <exception>
#include <memory>
#include "GameEventHandler.hpp"
#include "GameScene.hpp"
#include "IRenderer.hpp"
#include "InputEvents.hpp"
#include "NetClient.hpp"
#include "SFMLInputHandler.hpp"
#include "SFMLTextureManager.hpp"

/**
 * @namespace Game
 * @brief Contains all game-related classes and interfaces.
 */
namespace Game
{
    /**
     * @class GameClientError
     * @brief Exception class for client-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class GameClientError : public std::exception {
      public:
        /**
         * @brief Constructor for GameClientError.
         * @param message The error message to be associated with the exception.
         */
        explicit GameClientError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; //> Error message
    };

    /**
     * @class GameClient
     * @brief Main client class responsible for initializing and running the game.
     *
     * The GameClient manages the renderer, handles the game loop, and coordinates
     * between different subsystems like rendering, networking, and ECS.
     */
    class GameClient {
      public:
        /**
         * @brief Initializes the game client.
         *
         * Sets up the renderer and other necessary components.
         */
        void init(unsigned int width = 800, unsigned int height = 600);

        /**
         * @brief Runs the main game loop.
         *
         * Handles events, updates game state, and renders the scene.
         */
        void run();

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr;             ///> The renderer interface
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///> The texture manager interface
        std::shared_ptr<NetClient> _netClient = nullptr;                      ///> The network client
        std::shared_ptr<GameScene> _gameScene = nullptr;                      ///> The main game scene

        std::shared_ptr<Events::InputEventManager> _eventManager = nullptr;   ///> Event manager for input events
        std::shared_ptr<Input::SFMLInputHandler> _inputHandler = nullptr;     ///> Modern SFML input handler
        std::shared_ptr<Input::GameEventHandler> _gameInputHandler = nullptr; ///> Game-specific input handler
        Ecs::Entity _playerEntity{0};                                         ///> The player entity
    };

} // namespace Game
