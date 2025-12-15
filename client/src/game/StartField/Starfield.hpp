/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Starfield
*/

#pragma once

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include "IRenderer.hpp"
#include "ITextureManager.hpp"

/**
 * @namespace Background
 * @brief Contains all background-related classes and interfaces.
 */
namespace Background
{
    /**
     * @class StarfieldError
     * @brief Exception class for starfield-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class StarfieldError : public std::exception {
      public:
        /**
         * @brief Constructor for StarfieldError.
         * @param message The error message to be associated with the exception.
         */
        explicit StarfieldError(const std::string &message) : _message("\n\t" + message)
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
     * @class Starfield
     * @brief Class responsible for managing and rendering the scrolling starfield background.
     *
     * This class handles the background scrolling effect by managing two background sprites
     * that continuously scroll to create an infinite background effect.
     */
    class Starfield {
      public:
        /**
         * @brief Constructor for Starfield.
         * @param renderer Shared pointer to the renderer.
         * @param textureManager Shared pointer to the texture manager.
         */
        Starfield(
            std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager);

        /**
         * @brief Destructor for Starfield.
         */
        ~Starfield() = default;

        /**
         * @brief Updates the starfield animation.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(float deltaTime);

        /**
         * @brief Renders the starfield to the screen.
         */
        void render();

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer;             ///> The renderer interface
        std::shared_ptr<Graphics::ITextureManager> _textureManager; ///> The texture manager interface

        float _scrollSpeed = 100.0f;   ///> Speed of background scrolling in pixels per second
        float _background1X = 0.0f;    ///> X position of first background sprite
        float _background2X = 0.0f;    ///> X position of second background sprite
        float _backgroundWidth = 0.0f; ///> Width of the background texture
        float _scaleY = 1.0f;          ///> Y scale factor to fit window height

        std::unique_ptr<Graphics::ISprite> _sprite1; ///> First background sprite
        std::unique_ptr<Graphics::ISprite> _sprite2; ///> Second background sprite

        const std::string BACKGROUND_TEXTURE_PATH = "client/assets/sprites/background_space.png";
    };

} // namespace Background