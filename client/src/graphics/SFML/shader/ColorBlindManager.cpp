/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ColorBlindManager
*/

#include "ColorBlindManager.hpp"

namespace Graphics
{

    ColorBlindManager::ColorBlindManager(const std::shared_ptr<Resources::IResourceManager> &resourceManager)
        : _mode(ColorBlindMode::NONE), _shaderLoaded(false)
    {
        if (!sf::Shader::isAvailable()) {
            std::cerr << "ColorBlindManager: Shaders are not supported on this system\n";
            _shaderLoaded = false;
            return;
        }
        const auto [data, size] = resourceManager->loadResource("shaders/colorblind.frag");
        if (!data || size == 0) {
            _shaderLoaded = false;
            std::cerr << "ColorBlindManager: Failed to load colorblind shader resource\n";
            return;
        }
        const std::string shaderCode(reinterpret_cast<const char *>(data), size);
        if (!_shader.loadFromMemory(shaderCode, sf::Shader::Type::Fragment)) {
            _shaderLoaded = false;
            std::cerr << "ColorBlindManager: Failed to compile colorblind shader\n";
            return;
        }
        _shaderLoaded = true;
        _shader.setUniform("texture", sf::Shader::CurrentTexture);
        _shader.setUniform("mode", 0);
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

    ColorBlindMode ColorBlindManager::getMode() const noexcept
    {
        return _mode;
    }

    const sf::Shader *ColorBlindManager::getShader() const
    {
        return _shaderLoaded ? &_shader : nullptr;
    }

    bool ColorBlindManager::isShaderAvailable() const noexcept
    {
        return _shaderLoaded && _mode != ColorBlindMode::NONE;
    }

} // namespace Graphics