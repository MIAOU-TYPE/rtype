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

    ColorBlindManager::ColorBlindManager() : _mode(ColorBlindMode::NONE), _shaderLoaded(false)
    {
        if (!sf::Shader::isAvailable()) {
            throw ColorBlindError("ColorBlindManager: Shaders are not supported on this system");
        }

        if (!_shader.loadFromFile("client/assets/shaders/colorblind.frag", sf::Shader::Type::Fragment)) {
            _shaderLoaded = false;
            throw ColorBlindError("ColorBlindManager: Failed to load colorblind shader");
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