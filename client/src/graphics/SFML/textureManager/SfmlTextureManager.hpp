/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlTextureManager
*/
#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>

#include "IResourceManager.hpp"
#include "ITextureManager.hpp"

namespace Graphics
{
    /**
     * @class SfmlTextureManager
     * @brief SFML implementation of the ITextureManager interface.
     * This class manages texture loading, unloading, and retrieval using SFML.
     */
    class SfmlTextureManager : public ITextureManager {
      public:
        /**
         * @brief Constructor for SfmlTextureManager.
         * @param resources Reference to the resource manager for loading texture data.
         */
        explicit SfmlTextureManager(std::shared_ptr<Resources::IResourceManager> resources);

        /**
         * @brief Destructor for SfmlTextureManager.
         */
        [[nodiscard]] TextureHandle load(const std::string &resourcePath) override;

        /**
         * @brief Unload a texture by its handle.
         * @param handle The handle of the texture to unload.
         */
        void unload(TextureHandle handle) override;

        /**
         * @brief Check if a texture handle is valid.
         * @param handle The texture handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] bool isValid(TextureHandle handle) const noexcept override;

        /**
         * @brief Get the size of a texture by its handle.
         * @param handle The texture handle.
         * @return TextureSize structure containing width and height.
         */
        [[nodiscard]] TextureSize getSize(TextureHandle handle) const override;

        /**
         * @brief Clear all loaded textures.
         */
        void clear() override;

        /**
         * @brief Get the SFML texture associated with a texture handle.
         * @param handle The texture handle.
         * @return Reference to the SFML texture.
         */
        const sf::Texture &get(TextureHandle handle) const;

      private:
        /**
         * @struct TextureEntry
         * @brief Represents a loaded texture and its reference count.
         */
        struct TextureEntry {
            sf::Texture texture;      ///> The SFML texture
            std::size_t refCount = 1; ///> Reference count for the texture
        };

        std::shared_ptr<Resources::IResourceManager> _resources =
            nullptr; ///> Resource manager for loading texture data

        std::unordered_map<TextureHandle, TextureEntry> _textures;    ///> Map of texture handles to texture entries
        std::unordered_map<std::string, TextureHandle> _pathToHandle; ///> Map of resource paths to texture handles

        TextureHandle _nextHandle = 1; ///> Next available texture handle
    };
} // namespace Graphics
