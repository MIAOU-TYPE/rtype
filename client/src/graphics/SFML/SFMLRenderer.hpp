/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLRenderer
*/

#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "../interfaces/IRenderer.hpp"

/**
 * @class SFMLRenderer
 * @brief SFML implementation of the IRenderer interface.
 *
 * This class provides rendering functionality using the SFML library.
 */
class SFMLRenderer : public IRenderer {
private:
    sf::RenderWindow window; ///< The SFML render window

public:
    /**
     * @brief Creates the SFML window.
     * @param width The width of the window.
     * @param height The height of the window.
     * @param title The title of the window.
     */
    void createWindow(int width, int height, const std::string& title) override;

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
    bool pollEvent(sf::Event& event) override;
};

#endif // SFML_RENDERER_HPP