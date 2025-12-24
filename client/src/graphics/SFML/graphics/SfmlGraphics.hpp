/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlGraphics
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <iostream>
#include <memory>
#include "EmbeddedResourceManager.hpp"
#include "Event.hpp"
#include "IGraphics.hpp"
#include "Key.hpp"
#include "SfmlRenderer.hpp"
#include <unordered_map>

namespace Graphics
{
    /**
     * @class SfmlGraphics
     * @brief SFML implementation of the IGraphics interface.
     * This class provides graphics functionality using the SFML library.
     */
    class SfmlGraphics final : public IGraphics {
      public:
        /**
         * @brief Destructor for SfmlGraphics.
         */
        ~SfmlGraphics() override = default;

        /**
         * @brief Create a window with specified dimensions, title, and fullscreen mode.
         * @param size The size of the window as an Extent2u structure.
         * @param title The title of the window.
         * @param fullscreen Whether to create the window in fullscreen mode.
         */
        void create(Extent2u size, const std::string &title, bool fullscreen) override;

        /**
         * @brief Close the window.
         */
        void close() noexcept override;

        /**
         * @brief Check if the window is open.
         * @return True if the window is open, false otherwise.
         */
        [[nodiscard]] bool isOpen() const noexcept override;

        /**
         * @brief Set the resolution of the window.
         * @param size The new size of the window as an Extent2u structure.
         */
        void setResolution(Extent2u size) override;

        /**
         * @brief Poll for window events.
         */
        void pollEvents(Engine::EventBus &bus) override;

        /**
         * @brief Create a renderer for the graphics system.
         * @return Shared pointer to the created renderer.
         */
        std::shared_ptr<IRenderer> createRenderer() const noexcept override;

      private:
        std::shared_ptr<sf::RenderWindow> _window = nullptr;                     ///> The SFML render window
        std::shared_ptr<Resources::IResourceManager> _resourceManager = nullptr; ///> The resource manager
    };
} // namespace Graphics
