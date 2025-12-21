/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SfmlRenderer
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <utility>
#include "EmbeddedResourceManager.hpp"
#include "IRenderer.hpp"
#include "SfmlFontManager.hpp"
#include "SfmlTextureManager.hpp"
#include <iostream>

namespace Graphics
{
    /**
     * @class SfmlRenderer
     * @brief SFML implementation of the IRenderer interface.
     */
    class SfmlRenderer final : public IRenderer {
      public:
        /**
         * @brief Constructor for SfmlRenderer.
         * @param window Shared pointer to the SFML RenderWindow.
         * @param resourceManager Shared pointer to the resource manager.
         */
        explicit SfmlRenderer(const std::shared_ptr<sf::RenderWindow> &window,
            std::shared_ptr<Resources::IResourceManager> resourceManager);

        /**
         * @brief Begins a new rendering frame.
         */
        void beginFrame() override;

        /**
         * @brief Ends the current rendering frame.
         */
        void endFrame() override;

        /**
         * @brief Provides access to the font manager.
         * @return Reference to the font manager.
         */
        std::shared_ptr<IFontManager> fonts() const noexcept override;

        /**
         * @brief Provides access to the texture manager.
         * @return Reference to the texture manager.
         */
        std::shared_ptr<ITextureManager> textures() const noexcept override;

        /**
         * @brief Draws a render command.
         * @param cmd The render command to draw.
         */
        void draw(const Engine::RenderCommand &cmd) override;

      private:
        std::shared_ptr<sf::RenderWindow> _window = nullptr; ///> Shared pointer to the SFML RenderWindow.

        std::shared_ptr<Resources::IResourceManager> _resourceManager =
            nullptr;                                                   ///> Shared pointer to the resource manager.
        std::shared_ptr<SfmlFontManager> _fontManager = nullptr;       ///> Shared pointer to the SFML Font Manager.
        std::shared_ptr<SfmlTextureManager> _textureManager = nullptr; ///> Shared pointer to the SFML Texture Manager.
    };
} // namespace Graphics
