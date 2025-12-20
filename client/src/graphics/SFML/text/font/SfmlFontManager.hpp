/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** sfmlFontManager
*/

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "IFontManager.hpp"
#include "IResourceManager.hpp"

namespace Graphics
{
    /**
     * @class SfmlFontManager
     * @brief SFML implementation of the IFontManager interface.
     * This class manages font loading, unloading, and retrieval using SFML.
     */
    class SfmlFontManager final : public IFontManager {
      public:
        /**
         * @brief Constructor for SfmlFontManager.
         * @param resources Shared pointer to the resource manager for loading font data.
         */
        explicit SfmlFontManager(std::shared_ptr<Resources::IResourceManager> resources);

        /**
         * @brief Destructor for SfmlFontManager.
         */
        ~SfmlFontManager() override = default;

        /**
         * @brief Load a font from a resource identifier.
         * @param resourcePath Path or identifier of the font resource.
         * @return FontHandle Handle to the font, or InvalidFont on failure.
         */
        FontHandle load(const std::string &resourcePath) noexcept override;

        /**
         * @brief Unload a font.
         * @param handle The handle of the font to unload.
         */
        void unload(FontHandle handle) noexcept override;

        /**
         * @brief Check if a font handle is valid.
         * @param handle The font handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] bool isValid(FontHandle handle) const noexcept override;

        /**
         * @brief Clear all loaded fonts.
         */
        void clear() noexcept override;

        /**
         * @brief Get the SFML font associated with a font handle.
         * @param handle The font handle.
         * @return Reference to the SFML font.
         */
        const sf::Font &get(FontHandle handle) const;

      private:
        /**
         * @struct FontEntry
         * @brief Internal structure to hold font data and reference count.
         */
        struct FontEntry {
            sf::Font font;            ///> The SFML font
            std::size_t refCount = 1; ///> Reference count for the font
        };

        std::shared_ptr<const Resources::IResourceManager> _resources =
            nullptr; ///> Resource manager for loading font data

        std::unordered_map<FontHandle, FontEntry> _fonts;          ///> Map of font handles to font entries
        std::unordered_map<std::string, FontHandle> _pathToHandle; ///> Map of resource paths to font handles

        FontHandle _nextHandle = 1; ///> Next available font handle
    };
} // namespace Graphics
