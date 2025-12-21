/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ITextureManager
*/

#pragma once

#include <cstddef>
#include <exception>
#include <memory>
#include <string>

namespace Graphics
{

    class TextureError : public std::exception {
      public:
        /**
         * @brief Constructor for TextureError.
         * @param message The error message.
         */
        explicit TextureError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Override of what() method from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Opaque handle to a texture.
     * Backend-specific implementations will internally map this
     * handle to a real GPU texture (sf::Texture, OpenGL id, etc.)
     */
    using TextureHandle = std::size_t;
    constexpr TextureHandle InvalidTexture = 0;

    /**
     * @class ITextureManager
     * @brief Interface for managing textures independently of the graphics backend.
     */
    class ITextureManager {
      public:
        /**
         * @brief Virtual destructor for ITextureManager.
         */
        virtual ~ITextureManager() = default;

        /**
         * @brief Load a texture from a resource identifier.
         * If the texture is already loaded, returns the existing handle.
         * @param resourcePath Path or identifier of the texture.
         * @return TextureHandle Handle to the texture, or InvalidTexture on failure.
         */
        [[nodiscard]] virtual TextureHandle load(const std::string &resourcePath) = 0;

        /**
         * @brief Unload a texture.
         * Reference-counting or deferred destruction is backend-specific.
         */
        virtual void unload(TextureHandle handle) = 0;

        /**
         * @brief Check if a texture handle is valid.
         */
        [[nodiscard]] virtual bool isValid(TextureHandle handle) const noexcept = 0;

        /**
         * @brief Release all loaded textures.
         */
        virtual void clear() = 0;
    };
} // namespace Graphics
