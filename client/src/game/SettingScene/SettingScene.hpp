/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SettingScene
*/

#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include "Button.hpp"
#include "EmbeddedResourceManager.hpp"
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "SFMLAudio.hpp"
#include "SFMLEntityDrawing.hpp"

/**
 * @namespace Game
 * @brief Contains all game-related classes and interfaces.
 */
namespace Game
{
    class SettingSceneError : public std::exception {
      public:
        /**
         * @brief Constructor for SettingSceneError.
         * @param message The error message to be associated with the exception.
         */
        explicit SettingSceneError(const std::string &message) : _message("\n\t" + message)
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
     * @class SettingScene
     * @brief Represents the settings scene of the game.
     */
    class SettingScene {
      public:
        /**
         * @brief Constructor for SettingScene.
         */
        SettingScene(std::shared_ptr<Graphics::IRenderer> renderer,
            std::shared_ptr<Graphics::ITextureManager> textureManager, std::shared_ptr<Audio::SFMLAudio> audioManager);

        /**
         * @brief Initializes the buttons in the settings scene.
         */
        void initButtons(float backButtonCenterX, unsigned int windowHeight, const std::string &fontPath);

        /**
         * @brief Sets up the sound control buttons.
         */
        void setSoundButtons();

        /**
         * @brief Destructor for SettingScene.
         */
        void update(float mouseX, float mouseY, bool isMouseClicked);

        /**
         * @brief Renders the settings scene.
         */
        void render();
        /**
         * @brief Sets the callback for when Back button is clicked.
         * @param callback The callback function.
         */
        void setOnBackCallback(std::function<void()> callback);

        /**
         * @brief Sets the error message to be displayed.
         * @param message The error message string.
         */
        void setErrorMessage(const std::string &message);

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr;             ///> Shared pointer to the renderer
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///> Shared pointer to the texture manager
        std::unique_ptr<Graphics::ISprite> _backgroundSprite = nullptr; ///> Unique pointer to the background sprite
        std::unique_ptr<Graphics::IText> _errorText = nullptr;          ///> Unique pointer to the error text
        std::unique_ptr<Button> _backButton = nullptr;                  ///> Unique pointer to the Back button
        std::unique_ptr<Button> _increaseVolumeButton = nullptr; ///> Unique pointer to the Increase Volume button
        std::unique_ptr<Button> _decreaseVolumeButton = nullptr; ///> Unique pointer to the Decrease Volume button
        std::shared_ptr<Audio::SFMLAudio> _audio = nullptr;      ///> Shared pointer to the audio manager
        std::string _errorMessage = "";                          ///> Error message string
    };

} // namespace Game