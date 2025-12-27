/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IRenderer
*/

#pragma once
#include <exception>
#include <string>
#include "GraphicsTypes.hpp"
#include "IAudioManager.hpp"
#include "IFontManager.hpp"
#include "IText.hpp"
#include "ITextManager.hpp"
#include "ITextureManager.hpp"
#include "RenderCommand.hpp"

namespace
{
    constexpr float REF_WIDTH = 1920.f;
    constexpr float REF_HEIGHT = 1080.f;
} // namespace

namespace Graphics
{
    class RenderException : public std::exception {
      public:
        explicit RenderException(const std::string &message) : _message("\n\t" + message)
        {
        }

        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @class IRenderer
     * @brief Interface for a renderer responsible for managing the rendering process.
     * This interface defines methods to begin and end a rendering frame.
     */
    class IRenderer {
      public:
        /**
         * @brief Virtual destructor for IRenderer.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~IRenderer() = default;

        /**
         * @brief Retrieves the current size of the viewport.
         * @return ViewportSize struct containing width and height of the viewport.
         */
        virtual Extent2u getViewportSize() const noexcept = 0;

        /**
         * @brief Begins a new rendering frame.
         * This method should be called before any rendering operations for the frame.
         */
        virtual void beginFrame() = 0;

        /**
         * @brief Ends the current rendering frame.
         * This method should be called after all rendering operations for the frame are complete.
         */
        virtual void endFrame() = 0;

        /**
         * @brief Provides access to the font manager.
         * @return Reference to the font manager.
         */
        virtual std::shared_ptr<IFontManager> fonts() const noexcept = 0;

        /**
         * @brief Provides access to the texture manager.
         * @return Reference to the texture manager.
         */
        virtual std::shared_ptr<ITextureManager> textures() const noexcept = 0;

        /**
         * @brief Provides access to the text manager.
         * @return Reference to the text manager.
         */
        [[nodiscard]] virtual std::shared_ptr<ITextManager> texts() const noexcept = 0;

        /**
         * @brief Provides access to the sound manager.
         * @return Reference to the sound manager.
         */
        virtual std::shared_ptr<IAudioManager> audioSound() const noexcept = 0;

        /**
         * @brief Provides access to the music manager.
         * @return Reference to the music manager.
         */
        virtual std::shared_ptr<IAudioManager> audioMusic() const noexcept = 0;

        /**
         * @brief Draw a sprite based on the provided SpriteCmd.
         * @param cmd The command containing sprite drawing parameters.
         */
        virtual void draw(const Engine::RenderCommand &cmd) = 0;

        /**
         * @brief Draw text based on the provided IText object.
         * @param text The text object containing text drawing parameters.
         */
        virtual void drawText(const IText &text) = 0;
    };
} // namespace Graphics
