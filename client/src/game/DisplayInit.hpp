/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** DisplayInit
*/

#pragma once

#include <memory>
#include "EmbeddedResourceManager.hpp"
#include "GameScene.hpp"
#include "IRenderer.hpp"
#include "ITextureManager.hpp"
#include "MenuScene.hpp"
#include "SFMLRenderer.hpp"
#include "SFMLTextureManager.hpp"

/**
 * @namespace Display
 * @brief Contains all display initialization and management classes.
 */
namespace Display
{
    /**
     * @enum SceneState
     * @brief Represents the current scene state.
     */
    enum class SceneState {
        Menu,    ///> Main menu scene
        Gameplay ///> Gameplay scene
    };

    /**
     * @class DisplayInitError
     * @brief Exception class for client-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class DisplayInitError : public std::exception {
      public:
        /**
         * @brief Constructor for DisplayInitError.
         * @param message The error message to be associated with the exception.
         */
        explicit DisplayInitError(const std::string &message) : _message("\n\t" + message)
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

    class DisplayInit {
      public:
      /** @brief Default constructor for DisplayInit. */
        DisplayInit();

        /**
         * @brief Constructor for DisplayInit.
         * @param renderer Shared pointer to the renderer interface.
         */
        explicit DisplayInit(std::shared_ptr<Graphics::IRenderer> renderer);
        
        /** @brief Destructor for DisplayInit. */
        ~DisplayInit();

        /**
         * @brief Runs the main display loop.
         */
        void run();

        /**
         * @brief Initializes the display components.
         * @param width The width of the window.
         * @param height The height of the window.
         */
        void init(unsigned int width, unsigned int height);

        /**
         * @brief Checks if the window is still open.
         * @return True if the window is open, false otherwise.
         */
        bool isWindowOpen() const;

        /**
         * @brief Gets the renderer instance.
         * @return Shared pointer to the renderer.
         */
        std::shared_ptr<Graphics::IRenderer> getRenderer() const;

      protected:
      private:
        std::shared_ptr<Graphics::IRenderer> _renderer = nullptr;             ///> The renderer interface
        std::shared_ptr<Graphics::ITextureManager> _textureManager = nullptr; ///> The texture manager interface
        std::shared_ptr<Game::GameScene> _gameScene = nullptr;                ///> The main game scene
        std::shared_ptr<Game::MenuScene> _menuScene = nullptr;                ///> The menu scene
        SceneState _currentScene = SceneState::Menu;                          ///> Current active scene
    };
} // namespace Display
