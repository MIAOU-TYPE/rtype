/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLInputHandler
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "interfaces/IInputHandler.hpp"

namespace Input
{

    /**
     * @class SFMLInputHandler
     * @brief SFML implementation of the IInputHandler interface.
     *
     * This class provides input handling functionality using the SFML library.
     */
    class SFMLInputHandler : public IInputHandler {
      public:
        /**
         * @brief Handles an input event.
         * @param event The input event to handle.
         */
        void handleEvent(const sf::Event &event) override;

        /**
         * @brief Checks if a key is currently pressed.
         * @param key The key to check.
         * @return True if the key is pressed, false otherwise.
         */
        bool isKeyPressed(Key key) const override;
    };

} // namespace Input
