/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameOverScene
*/

#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include "Button.hpp"
#include "IRenderer.hpp"
#include "ITextureManager.hpp"

/**
 * @namespace Game
 * @brief Contains all game-related classes and interfaces.
 */
namespace Game
{
    /**
     * @class GameOverSceneError
     * @brief Exception class for game over scene-related errors.
     */
    class GameOverSceneError : public std::exception {
      public:
        /**
         * @brief Constructor for GameOverSceneError.
         * @param message The error message to be associated with the exception.
         */
        explicit GameOverSceneError(const std::string &message) : _message("\n\t" + message)
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
     * @class GameOverScene
     * @brief Represents the game over scene displayed when the game ends.
     */
    class GameOverScene {
      public:
        /**
         * @brief Constructor for GameOverScene.
         * @param renderer Shared pointer to the renderer.
         * @param textureManager Shared pointer to the texture manager.
         */
        GameOverScene(
            std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager);

        /**
         * @brief Updates the game over scene.
         * @param mouseX The X position of the mouse.
         * @param mouseY The Y position of the mouse.
         * @param isMouseClicked Whether the mouse button is clicked.
         */
        void update(float mouseX, float mouseY, bool isMouseClicked);

        /**
         * @brief Renders the game over scene.
         */
        void render();

        /**
         * @brief Sets the callback for when Return to Menu button is clicked.
         * @param callback The callback function.
         */
        void setOnReturnToMenuCallback(std::function<void()> callback);

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr;             ///> The renderer
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///> The texture manager
        std::unique_ptr<Graphics::ISprite> _backgroundSprite = nullptr;       ///> Background sprite
        std::unique_ptr<Button> _returnToMenuButton = nullptr;                ///> Return to Menu button
    };

} // namespace Game