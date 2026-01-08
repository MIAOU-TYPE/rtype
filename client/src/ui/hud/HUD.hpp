/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HUD
*/

#pragma once

#include <memory>
#include "ClientWorld.hpp"
#include "IFontManager.hpp"
#include "IRenderer.hpp"
#include "IText.hpp"

namespace Engine
{
    /**
     * @brief Handles the rendering of the Heads-Up Display (HUD) in the game.
     */
    class HUD {
      public:
        /**
         * @brief Constructs a HUD with the given renderer and world.
         * @param renderer Shared pointer to the renderer.
         * @param world Reference to the client world.
         */
        explicit HUD(std::shared_ptr<Graphics::IRenderer> renderer, World::ClientWorld &world);

        /**
         * @brief Initializes the HUD, loading fonts and creating text objects.
         */
        void initialize();

        /**
         * @brief Renders the HUD elements.
         */
        void render();

      private:
        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Shared pointer to the renderer.
        World::ClientWorld &_world;                     ///> Reference to the client world.
        Graphics::FontHandle _fontHandle;               ///> Handle to the font for HUD text.
        std::unique_ptr<Graphics::IText> _scoreText;    ///> Text object for displaying the score.
    };
} // namespace Engine