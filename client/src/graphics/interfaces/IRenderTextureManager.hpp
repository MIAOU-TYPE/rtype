/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IRenderTextureManager
*/

#pragma once

#include <exception>
#include <string>

namespace Graphics
{
    class RenderTextureError : public std::exception {
      public:
        /**
         * @brief Constructor for RenderTextureError.
         * @param message The error message.
         */
        explicit RenderTextureError(const std::string &message) : _message("\n\t" + message)
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
     * @class IRenderTextureManager
     * @brief Interface for managing render textures for post-processing effects.
     */
    class IRenderTextureManager {
      public:
        virtual ~IRenderTextureManager() = default;

        /**
         * @brief Check if render texture is available and initialized.
         * @return True if render texture is ready to use.
         */
        [[nodiscard]] virtual bool isAvailable() const noexcept = 0;

        /**
         * @brief Initialize or resize the render texture to match window size.
         * @param width New width for the render texture.
         * @param height New height for the render texture.
         * @return True if resize was successful.
         */
        virtual bool resize(unsigned int width, unsigned int height) = 0;

        /**
         * @brief Clear the render texture.
         */
        virtual void clear() = 0;

        /**
         * @brief Finalize rendering to texture and prepare for display.
         */
        virtual void display() = 0;

        /**
         * @brief Disable the render texture.
         */
        virtual void disable() = 0;
    };
} // namespace Graphics
