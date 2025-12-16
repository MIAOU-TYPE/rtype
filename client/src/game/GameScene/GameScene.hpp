/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameScene
*/

#pragma once

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include "EmbeddedResourceManager.hpp"
#include "GameHUD.hpp"
#include "GameWorld.hpp"
#include "IHUD.hpp"
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "InputSystem.hpp"
#include "Registry.hpp"
#include "SFMLAudio.hpp"
#include "SFMLEntityDrawing.hpp"
#include "Starfield.hpp"

/**
 * @namespace Game
 * @brief Contains all game-related classes and interfaces.
 */
namespace Game
{
    /**
     * @class GameSceneError
     * @brief Exception class for game scene-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class GameSceneError : public std::exception {
      public:
        /**
         * @brief Constructor for GameSceneError.
         * @param message The error message to be associated with the exception.
         */
        explicit GameSceneError(const std::string &message) : _message("\n\t" + message)
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
        std::string _message = ""; ///> Error message
    };

    /**
     * @class GameScene
     * @brief Main game scene class responsible for managing and rendering game objects.
     *
     * The GameScene handles the game logic, updates entities, and renders the scene
     * including the background, entities, and UI elements.
     */
    class GameScene {
      public:
        /**
         * @brief Constructor for GameScene.
         * @param renderer Shared pointer to the renderer.
         * @param textureManager Shared pointer to the texture manager.
         */
        GameScene(
            std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager);

        /**
         * @brief Destructor for GameScene.
         */
        ~GameScene() = default;

        /**
         * @brief Updates the game scene.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(float deltaTime) const;

        /**
         * @brief Renders the game scene.
         */
        void render() const;

        /**
         * @brief Gets the ECS registry.
         * @return Reference to the ECS registry.
         */
        Ecs::Registry &getRegistry();

        /**
         * @brief Gets the game world.
         * @return Reference to the game world.
         */
        GameWorld &getGameWorld() const;

        /**
         * @brief Gets a shared pointer to the game world.
         * @return Shared pointer to the game world.
         */
        std::shared_ptr<GameWorld> getGameWorldPtr();

        /**
         * @brief Gets the HUD interface.
         * @return Pointer to the HUD interface.
         */
        Graphics::IHUD *getHUD();

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr;             ///> The renderer interface
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///> The texture manager interface
        std::unique_ptr<Background::Starfield> _starfield = nullptr;          ///> The starfield background

        std::unique_ptr<Graphics::GameHUD> _hud = nullptr;        ///> The game HUD
        Ecs::Registry _registry;                                  ///> The ECS registry
        std::shared_ptr<GameWorld> _gameWorld;                    ///> The game world
        std::unique_ptr<Ecs::InputSystem> _inputSystem = nullptr; ///> The input system
        std::shared_ptr<Resources::EmbeddedResourceManager> _audioResourceManager =
            nullptr;                                               ///> Resource manager for audio
        std::unique_ptr<Audio::SFMLAudio> _audioManager = nullptr; ///> The audio manager
        // std::unique_ptr<Graphics::SFMLEntityDrawing> _entityDrawing = nullptr; ///> Entity drawing manager
    };

} // namespace Game
