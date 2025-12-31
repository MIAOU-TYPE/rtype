/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SfmlRenderer
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>
#include <utility>
#include "ColorBlindManager.hpp"
#include "EmbeddedResourceManager.hpp"
#include "IRenderer.hpp"
#include "SfmlFontManager.hpp"
#include "SfmlMusicManager.hpp"
#include "SfmlSoundManager.hpp"
#include "SfmlTextManager.hpp"
#include "SfmlTextureManager.hpp"

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
         * @brief Gets the size of the viewport.
         * @return The size of the viewport as Extent2u.
         */
        [[nodiscard]] Extent2u getViewportSize() const noexcept override;

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
         * @brief Provides access to the text manager.
         * @return Reference to the text manager.
         */
        [[nodiscard]] std::shared_ptr<ITextManager> texts() const noexcept override;

        /**
         * @brief Provides access to the sound manager.
         * @return Reference to the sound manager.
         */
        std::shared_ptr<IAudioManager> sounds() const noexcept override;

        /**
         * @brief Provides access to the music manager.
         * @return Reference to the music manager.
         */
        std::shared_ptr<IAudioManager> musics() const noexcept override;

        /**
         * @brief Draws a render command.
         * @param cmd The render command to draw.
         */
        void draw(const Engine::RenderCommand &cmd) override;

        /**
         * @brief Draws text.
         * @param text The text to draw.
         */
        void draw(const IText &text) override;

        /**
         * @brief Set the colorblind filter mode.
         * @param mode The colorblind mode to apply.
         */
        void setColorBlindMode(ColorBlindMode mode) override;

        /**
         * @brief Get the current colorblind filter mode.
         * @return ColorBlindMode The current mode.
         */
        [[nodiscard]] ColorBlindMode getColorBlindMode() const noexcept override;

      private:
        std::shared_ptr<sf::RenderWindow> _window = nullptr; ///> Shared pointer to the SFML RenderWindow.

        std::shared_ptr<Resources::IResourceManager> _resourceManager =
            nullptr;                                                   ///> Shared pointer to the resource manager.
        std::shared_ptr<SfmlSoundManager> _soundManager = nullptr;     ///> Shared pointer to the SFML Sound Manager.
        std::shared_ptr<SfmlMusicManager> _musicManager = nullptr;     ///> Shared pointer to the SFML Music Manager.
        std::shared_ptr<SfmlFontManager> _fontManager = nullptr;       ///> Shared pointer to the SFML Font Manager.
        std::shared_ptr<SfmlTextureManager> _textureManager = nullptr; ///> Shared pointer to the SFML Texture Manager.
        std::shared_ptr<SfmlTextManager> _textManager = nullptr;       ///> Shared pointer to the SFML Text Manager.
        std::unique_ptr<ColorBlindManager> _colorBlindManager = nullptr; ///> Colorblind shader manager.
        std::unique_ptr<sf::RenderTexture> _renderTexture = nullptr;     ///> Render texture for shader post-processing.
    };
} // namespace Graphics
