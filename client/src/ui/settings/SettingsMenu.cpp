/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"
#include <stdexcept>

namespace Engine
{
    SettingsMenu::SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer) : _renderer(renderer)
    {
        const auto textures = renderer->textures();

        _backgroundTexture = textures->load("sprites/bg-preview.png");
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw std::runtime_error("SettingsMenu: failed to load background");

        _backgroundCmd.textureId = _backgroundTexture;

        _audio = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "AUDIO");
        _resolution = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "1280x720");
        _resolutionNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _back = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "BACK");
    }

    void SettingsMenu::update(const float mouseX, const float mouseY)
    {
        const auto vp = _renderer->getViewportSize();
        const float w = static_cast<float>(vp.width);
        const float h = static_cast<float>(vp.height);
        const float cx = w * 0.5f;
        const float cy = h * 0.5f;
        const float scale = std::min(w / REF_WIDTH, h / REF_HEIGHT);
        const auto texSize = _renderer->textures()->getSize(_backgroundTexture);

        _backgroundCmd.frame = {0, 0, static_cast<int>(texSize.width), static_cast<int>(texSize.height)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {w / static_cast<float>(texSize.width), h / static_cast<float>(texSize.height)};

        _audio->setPosition(cx - 400.f * scale, cy - 60.f * scale);
        _resolution->setPosition(cx + 150.f * scale, cy - 60.f * scale);
        _resolutionNext->setPosition(cx + (150.f + _resolution->bounds().w + 250.f) * scale, cy - 60.f * scale);
        _back->setPosition(cx - _back->bounds().w * 0.5f, h * 0.8f);

        _audio->update(mouseX, mouseY);
        _resolution->update(mouseX, mouseY);
        _resolutionNext->update(mouseX, mouseY);
        _back->update(mouseX, mouseY);
    }

    void SettingsMenu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _audio->render();
        _resolution->render();
        _resolutionNext->render();
        _back->render();
    }

    bool SettingsMenu::onMousePressed(const float x, const float y) const
    {
        _audio->onMousePressed(x, y);
        _resolution->onMousePressed(x, y);
        _resolutionNext->onMousePressed(x, y);
        _back->onMousePressed(x, y);
        return false;
    }

    bool SettingsMenu::onMouseReleased(const float x, const float y)
    {
        _audio->onMouseReleased(x, y);
        if (_resolutionNext->onMouseReleased(x, y)) {
            _currentResolution = (_currentResolution + 1) % _resolutions.size();
            _resolutionChanged = true;
            const auto &res = _resolutions[_currentResolution];
            _resolution->setLabel(std::to_string(res.width) + "x" + std::to_string(res.height));
            _resolutionNext->reset();
            return true;
        }
        if (_back->onMouseReleased(x, y)) {
            _backRequested = true;
            _back->reset();
            return true;
        }
        return false;
    }

    bool SettingsMenu::wantsBack() const noexcept
    {
        return _backRequested;
    }

    bool SettingsMenu::resolutionChanged() noexcept
    {
        if (!_resolutionChanged)
            return false;
        _resolutionChanged = false;
        return true;
    }

    Graphics::Extent2u SettingsMenu::currentResolution() const noexcept
    {
        return _resolutions[_currentResolution];
    }
} // namespace Engine