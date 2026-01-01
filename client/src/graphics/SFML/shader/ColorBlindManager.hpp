/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ColorBlindManager
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include <string>
#include <iostream>
#include "GraphicsTypes.hpp"
#include "IResourceManager.hpp"

namespace Graphics
{
    /**
     * @class ColorBlindManager
     * @brief Manages colorblind filters using GLSL shaders.
     */
    class ColorBlindManager {
      public:
        /**
         * @brief Construct a new ColorBlindManager.
         * @param resourceManager Shared pointer to the resource manager for loading shaders.
         */
        explicit ColorBlindManager(std::shared_ptr<Resources::IResourceManager> resourceManager);

        /**
         * @brief Destroy the ColorBlindManager.
         */
        ~ColorBlindManager() = default;

        /**
         * @brief Set the colorblind filter mode.
         * @param mode The colorblind mode to apply.
         */
        void setMode(ColorBlindMode mode);

        /**
         * @brief Get the current colorblind filter mode.
         * @return ColorBlindMode The current mode.
         */
        ColorBlindMode getMode() const;

        /**
         * @brief Get the shader for applying to render targets.
         * @return const sf::Shader* Pointer to the shader, or nullptr if not loaded.
         */
        const sf::Shader *getShader() const;

        /**
         * @brief Check if a shader is loaded and ready to use.
         * @return bool True if shader is loaded.
         */
        bool isShaderAvailable() const;

      private:
        ColorBlindMode _mode; ///> Current colorblind filter mode
        sf::Shader _shader;   ///> GLSL shader for colorblind filters
        bool _shaderLoaded;   ///> Whether the shader loaded successfully
    };

} // namespace Graphics