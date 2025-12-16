/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLRenderer
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>
#include "IRenderer.hpp"
#include "SFMLEvent.hpp"
#include "IResourceManager.hpp"

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{

    /**
     * @class SFMLRenderer
     * @brief SFML implementation of the IRenderer interface.
     *
     * This class provides rendering functionality using the SFML library.
     */
    class SFMLRenderer : public IRenderer {
      public:
        /**
         * @brief Constructor for SFMLRenderer.
         * @param resourceManager The resource manager for loading embedded assets.
         */
        explicit SFMLRenderer(std::shared_ptr<Resources::IResourceManager> resourceManager);
        /**
         * @brief Creates the SFML window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         */
        void createWindow(unsigned int width, unsigned int height, const std::string &title) override;

        /**
         * @brief Checks if the SFML window is open.
         * @return True if the window is open, false otherwise.
         */
        bool isOpen() const override;

        /**
         * @brief Closes the SFML window.
         */
        void close() override;

        /**
         * @brief Clears the SFML window.
         */
        void clear() override;

        /**
         * @brief Displays the rendered frame in the SFML window.
         */
        void display() override;

        /**
         * @brief Polls for SFML events.
         * @param event The event to fill if available.
         * @return True if an event was polled, false otherwise.
         */
        bool pollEvent(std::shared_ptr<IEvent> &event) override;

        /**
         * @brief Checks if the event is a window close event.
         * @param event The event to check.
         * @return True if the event is a window close event, false otherwise.
         */
        bool isWindowCloseEvent(const IEvent &event) const override;

        /**
         * @brief Draws a sprite to the render target.
         * @param sprite The sprite to draw.
         */
        void drawSprite(const sf::Sprite &sprite) override;

        /**
         * @brief Renders an ISprite to the screen.
         * @param sprite The sprite to render.
         */
        void renderSprite(const ISprite &sprite) override;

        /**
         * @brief Renders text to the screen.
         * @param text The text to render.
         */
        void renderText(const IText &text) override;

        /**
         * @brief Gets the window width.
         * @return The width of the window.
         */
        unsigned int getWindowWidth() const override;

        /**
         * @brief Gets the window height.
         * @return The height of the window.
         */
        unsigned int getWindowHeight() const override;

        /**
         * @brief Gets the current mouse position relative to the window.
         * @param x Reference to store the X coordinate.
         * @param y Reference to store the Y coordinate.
         */
        void getMousePosition(float &x, float &y) const override;

        /**
         * @brief Gets the elapsed time since last restart in seconds.
         * @return The elapsed time in seconds.
         */
        float getElapsedTime() const override;

        /**
         * @brief Restarts the internal clock.
         */
        void restartClock() override;

        /**
         * @brief Sets the mouse pressed state.
         * @param isPressed True if the mouse is pressed, false otherwise.
         */
        void setIsMousePressed(bool isPressed) override;

        /**
         * @brief Checks if the mouse is currently pressed.
         * @return True if the mouse is pressed, false otherwise.
         */
        bool getIsMousePressed() const override;

        /**
         * @brief Activates or deactivates the OpenGL context for the calling thread.
         * @param active True to activate, false to deactivate.
         */
        void setActive(bool active) override;

        /**
         * @brief Sets the color blind mode.
         * @param type The type of color blindness to simulate.
         */
        void setColorBlindMode(ColorBlindType type) override;

        /**
         * @brief Gets the current color blind mode.
         * @return The type of color blindness currently simulated.
         */
        ColorBlindType getColorBlindMode() const override;

      private:
        sf::RenderWindow _window;                                       ///> The SFML render window
        mutable sf::Clock _clock;                                       ///> Internal clock for timing
        bool _isMousePressed = false;                                   ///> Tracks if the mouse is currently pressed
        mutable std::mutex _mutex;                                      ///> Mutex for thread-safe access
        ColorBlindType _colorBlindMode = ColorBlindType::NONE;          ///> Current color blind mode
        sf::RenderTexture _renderTexture;                               ///> Off-screen render texture for color blind simulation
        sf::Shader _colorBlindShader;                                   ///> Shader for color blind effects
        bool _shaderLoaded = false;                                     ///> Flag to check if the shader loaded successfully
        std::shared_ptr<Resources::IResourceManager> _resourceManager;  ///> Resource manager for embedded assets
        sf::RenderTarget* _currentTarget = nullptr;                     ///> Current render target (window or render texture)
    };

} // namespace Graphics
