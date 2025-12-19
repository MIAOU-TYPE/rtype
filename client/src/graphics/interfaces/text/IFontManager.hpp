/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IFontManager
*/

#pragma once

#include <cstddef>
#include <exception>
#include <string>

namespace Graphics
{
    /**
     * @class FontManagerError
     * @brief Exception class for font manager errors.
     */
    class FontManagerError : public std::exception {
      public:
        /**
         * @brief Constructor for FontManagerError.
         * @param message Error message.
         */
        explicit FontManagerError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Get the error message.
         * @return const char* Error message string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Opaque handle to a font.
     */
    using FontHandle = std::size_t;
    constexpr FontHandle InvalidFont = 0;

    /**
     * @class IFontManager
     * @brief Interface for managing font resources.
     * This interface abstracts font loading and lifetime management
     * independently of the underlying graphics backend.
     */
    class IFontManager {
      public:
        virtual ~IFontManager() = default;

        /**
         * @brief Load a font from a resource identifier.
         * If the font is already loaded, returns the existing handle.
         * @param resourcePath Path or identifier of the font resource.
         * @return FontHandle Handle to the font, or InvalidFont on failure.
         */
        virtual FontHandle load(const std::string &resourcePath) noexcept = 0;

        /**
         * @brief Unload a font.
         * Reference counting or deferred destruction is backend-specific.
         */
        virtual void unload(FontHandle handle) noexcept = 0;

        /**
         * @brief Check if a font handle is valid.
         */
        [[nodiscard]] virtual bool isValid(FontHandle handle) const noexcept = 0;

        /**
         * @brief Release all loaded fonts.
         */
        virtual void clear() noexcept = 0;
    };
} // namespace Graphics
