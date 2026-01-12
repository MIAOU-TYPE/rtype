/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** IMenu
*/

#pragma once

#include <memory>
#include <stdexcept>
#include <initializer_list>

#include "IRenderer.hpp"
#include "InputState.hpp"
#include "RenderCommand.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @brief Interface for menu screens in the game.
     * This interface defines the basic functionality that all menu screens must implement,
     * including updating, rendering, and layout management.
     */
    class IMenu {
      public:
        /**
         * @brief Virtual destructor for IMenu.
         */
        virtual ~IMenu() = default;

        /**
         * @brief Update the menu state based on the input frame.
         * @param frame The current input frame.
         */
        virtual void update(const InputFrame &frame) = 0;

        /**
         * @brief Render the menu.
         */
        virtual void render() const = 0;

        /**
         * @brief Handle resizing of the menu.
         */
        virtual void layout() = 0;
    };
} // namespace Engine
