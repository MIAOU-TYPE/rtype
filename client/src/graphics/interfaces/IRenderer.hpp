/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** IRenderer
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <string>

namespace Graphics
{

    /**
     * @class IRenderer
     * @brief Interface for rendering operations.
     *
     * This interface abstracts the rendering backend, allowing different implementations
     * (e.g., SFML, SDL) to be used interchangeably.
     */
    class IRenderer {
      public:
        /**
         * @brief Virtual destructor for IRenderer.
         */
        virtual ~IRenderer() = default;

        /**
         * @brief Creates the rendering window.
         * @param width The width of the window.
         * @param height The height of the window.
         * @param title The title of the window.
         */
        virtual void createWindow(unsigned int width, unsigned int height, const std::string &title) = 0;

        /**
         * @brief Checks if the window is open.
         * @return True if the window is open, false otherwise.
         */
        virtual bool isOpen() const = 0;

        /**
         * @brief Closes the window.
         */
        virtual void close() = 0;

        /**
         * @brief Clears the window with the default color.
         */
        virtual void clear() = 0;

        /**
         * @brief Displays the rendered frame.
         */
        virtual void display() = 0;

        /**
         * @brief Polls for events.
         * @param event The event to fill if available.
         * @return True if an event was polled, false otherwise.
         */
        virtual bool pollEvent(sf::Event &event) = 0;

        /**
         * @brief Checks if the event is a window close event.
         * @param event The event to check.
         * @return True if the event is a window close event, false otherwise.
         */
        virtual bool isWindowCloseEvent(const sf::Event &event) const = 0;
    };

} // namespace Graphics
