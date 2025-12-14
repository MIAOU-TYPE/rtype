/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** MenuScene
*/

#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include "Button.hpp"
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "SFMLEntityDrawing.hpp"

/**
 * @namespace Game
 * @brief Contains all game-related classes and interfaces.
 */
namespace Game
{
    /**
     * @class MenuSceneError
     * @brief Exception class for menu scene-related errors.
     */
    class MenuSceneError : public std::exception {
      public:
        /**
         * @brief Constructor for MenuSceneError.
         * @param message The error message to be associated with the exception.
         */
        explicit MenuSceneError(const std::string &message) : _message("\n\t" + message)
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
     * @class MenuScene
     * @brief Represents the main menu scene of the game.
     */
    class MenuScene {
      public:
        /**
         * @brief Constructor for MenuScene.
         * @param renderer Shared pointer to the renderer.
         * @param textureManager Shared pointer to the texture manager.
         */
        MenuScene(
            std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager);

        /**
         * @brief Updates the menu scene.
         * @param mouseX The X position of the mouse.
         * @param mouseY The Y position of the mouse.
         * @param isMouseClicked Whether the mouse button is clicked.
         */
        void update(float mouseX, float mouseY, bool isMouseClicked);

        /**
         * @brief Renders the menu scene.
         */
        void render();

        /**
         * @brief Sets the callback for when Play button is clicked.
         * @param callback The callback function.
         */
        void setOnPlayCallback(std::function<void()> callback);

        /**
         * @brief Sets the callback for when Quit button is clicked.
         * @param callback The callback function.
         */
        void setOnQuitCallback(std::function<void()> callback);

        /**
         * @brief Sets an error message to display in the menu.
         * @param message The error message to display.
         */
        void setErrorMessage(const std::string &message);

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr;             ///> The renderer
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///> The texture manager
        std::unique_ptr<Graphics::ISprite> _backgroundSprite = nullptr;       ///> Background sprite
        std::unique_ptr<Button> _playButton = nullptr;                        ///> Play button
        std::unique_ptr<Button> _quitButton = nullptr;                        ///> Quit button
        std::unique_ptr<Graphics::IText> _errorText = nullptr;                ///> Error message text
        std::string _errorMessage = "";                                       ///> Error message string
    };

} // namespace Game
