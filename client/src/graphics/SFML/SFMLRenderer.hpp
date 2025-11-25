/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLRenderer
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "IRenderer.hpp"

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
        bool pollEvent(sf::Event &event) override;

        /**
         * @brief Checks if the event is a window close event.
         * @param event The event to check.
         * @return True if the event is a window close event, false otherwise.
         */
        bool isWindowCloseEvent(const sf::Event &event) const override;

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
         * @brief Gets the window width.
         * @return The width of the window.
         */
        unsigned int getWindowWidth() const override;

        /**
         * @brief Gets the window height.
         * @return The height of the window.
         */
        unsigned int getWindowHeight() const override;

      private:
        sf::RenderWindow _window; ///> The SFML render window
    };

} // namespace Graphics
