/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IGraphics
*/

#pragma once
#include <exception>
#include <string>
#include "EventBus.hpp"
#include "IRenderer.hpp"

namespace Graphics
{
    /**
     * @class IGraphicsError
     * @brief Exception class for graphics-related errors.
     */
    class IGraphicsError : public std::exception {
      public:
        /**
         * @brief Construct a new IGraphicsError object.
         * @param message The error message.
         */
        explicit IGraphicsError(const std::string &message) : _message("\t\n" + message)
        {
        }

        /**
         * @brief Get the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///> The error message
    };

    /**
     * @class IGraphics
     * @brief Interface for graphics systems.
     * This interface defines the basic operations required for a graphics system,
     * such as creating and managing a window, clearing the screen, and displaying content.
     */
    class IGraphics {
      public:
        /**
         * @brief Virtual destructor for IGraphics.
         */
        virtual ~IGraphics() = default;

        /**
         * @brief Create a window with specified dimensions, title, and fullscreen mode.
         * @param width The width of the window
         * @param height The height of the window
         * @param title  The title of the window.
         * @param fullscreen  Whether to create the window in fullscreen mode.
         */
        virtual void create(unsigned int width, unsigned int height, const std::string &title, bool fullscreen) = 0;

        /**
         * @brief Close the currently open window.
         */
        virtual void close() noexcept = 0;

        /**
         * @brief Check if the window is currently open.
         * @return True if the window is open, false otherwise.
         */
        [[nodiscard]] virtual bool isOpen() const noexcept = 0;

        /**
         * @brief Create and return a renderer instance.
         * @return Shared pointer to the created IRenderer instance.
         */
        virtual std::shared_ptr<IRenderer> createRenderer() const noexcept = 0;

        /**
         * @brief Poll for window events.
         */
        virtual void pollEvents(Engine::EventBus &bus) = 0;
    };
} // namespace Graphics