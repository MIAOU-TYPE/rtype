/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ColorBlindManager
*/

#include "ColorBlindManager.hpp"
#include <iostream>

namespace Graphics
{

    ColorBlindManager::ColorBlindManager(std::shared_ptr<Resources::IResourceManager> resourceManager)
        : _mode(ColorBlindMode::NONE), _shaderLoaded(false)
    {
        if (!sf::Shader::isAvailable()) {
            throw ColorBlindError("ColorBlindManager: Shaders are not supported on this system");
        }
        const auto shaderData = resourceManager->loadResource("shaders/colorblind.frag");
        if (!shaderData.data || shaderData.size == 0) {
            _shaderLoaded = false;
            throw ColorBlindError("ColorBlindManager: Failed to load colorblind shader from resources");
        }
        const std::string shaderCode(reinterpret_cast<const char*>(shaderData.data), shaderData.size);
        if (!_shader.loadFromMemory(shaderCode, sf::Shader::Type::Fragment)) {
            _shaderLoaded = false;
            throw ColorBlindError("ColorBlindManager: Failed to compile colorblind shader");
        }
        _shaderLoaded = true;
        _shader.setUniform("texture", sf::Shader::CurrentTexture);
        _shader.setUniform("mode", 0);
    }

    ColorBlindManager::~ColorBlindManager()
    {
    }

    void ColorBlindManager::setMode(ColorBlindMode mode)
    {
        int shaderMode = 0;
        _mode = mode;

        if (!_shaderLoaded) {
            return;
        }

        switch (mode) {
            case ColorBlindMode::NONE: shaderMode = 0; break;
            case ColorBlindMode::DEUTERANOPIA: shaderMode = 1; break;
            case ColorBlindMode::PROTANOPIA: shaderMode = 2; break;
            case ColorBlindMode::TRITANOPIA: shaderMode = 3; break;
        }

        _shader.setUniform("mode", shaderMode);
    }

    ColorBlindMode ColorBlindManager::getMode() const
    {
        return _mode;
    }

    const sf::Shader *ColorBlindManager::getShader() const
    {
        return _shaderLoaded ? &_shader : nullptr;
    }

    bool ColorBlindManager::isShaderAvailable() const
    {
        return _shaderLoaded && _mode != ColorBlindMode::NONE;
    }

} // namespace Graphics