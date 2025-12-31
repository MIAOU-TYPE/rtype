/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"

namespace Engine
{
    SettingsMenu::SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer) : _renderer(renderer)
    {
        const auto textures = renderer->textures();

        _backgroundTexture = textures->load("sprites/bg-preview.png");
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw SettingsMenuError("SettingsMenu: failed to load background texture");

        _backgroundCmd.textureId = _backgroundTexture;

        _audio = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "AUDIO");
        _colorBlindMode = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "NORMAL");
        _colorBlindNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _resolution = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "1280x720");
        _resolutionNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _back = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "BACK");
    }

    void SettingsMenu::onEnter()
    {
        _backRequested = false;
        _resolutionChanged = false;

        _audio->reset();
        _colorBlindMode->reset();
        _colorBlindNext->reset();
        _resolution->reset();
        _resolutionNext->reset();
        _back->reset();

        layout();
    }

    void SettingsMenu::layout()
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

        _audio->setPosition(cx - 400.f * scale, cy - 120.f * scale);
        _colorBlindMode->setPosition(cx - 400.f * scale, cy + 40.f * scale);
        _colorBlindNext->setPosition(cx + (150.f + _colorBlindMode->bounds().w + 250.f) * scale, cy + 40.f * scale);
        _resolution->setPosition(cx - 400.f * scale, cy - 260.f * scale);
        _resolutionNext->setPosition(cx + (150.f + _resolution->bounds().w + 250.f) * scale, cy - 260.f * scale);
        _back->setPosition(cx - _back->bounds().w * 0.5f, h * 0.8f);
    }

    void SettingsMenu::update(const InputFrame &frame)
    {
        handleInput(frame);
        _audio->update(frame.mouseX, frame.mouseY);
        _colorBlindMode->update(frame.mouseX, frame.mouseY);
        _colorBlindNext->update(frame.mouseX, frame.mouseY);
        _resolution->update(frame.mouseX, frame.mouseY);
        _resolutionNext->update(frame.mouseX, frame.mouseY);
        _back->update(frame.mouseX, frame.mouseY);
    }

    void SettingsMenu::handleInput(const InputFrame &frame)
    {
        if (frame.keyPressed) {
            switch (frame.key) {
                case Key::B: _backRequested = true; break;
                case Key::Backspace: {
                    _currentResolution = (_currentResolution + 1) % _resolutions.size();
                    const auto &res = _resolutions.at(_currentResolution);
                    _resolution->setLabel(std::to_string(res.width) + "x" + std::to_string(res.height));
                    _resolutionChanged = true;
                    break;
                }
                default: break;
            }
        }

        if (frame.mousePressed) {
            _audio->onMousePressed(frame.mouseX, frame.mouseY);
            _colorBlindMode->onMousePressed(frame.mouseX, frame.mouseY);
            _colorBlindNext->onMousePressed(frame.mouseX, frame.mouseY);
            _resolution->onMousePressed(frame.mouseX, frame.mouseY);
            _resolutionNext->onMousePressed(frame.mouseX, frame.mouseY);
            _back->onMousePressed(frame.mouseX, frame.mouseY);
        }
        if (frame.mouseReleased) {
            (void) _audio->onMouseReleased(frame.mouseX, frame.mouseY);

            if (_colorBlindNext->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _currentColorBlindMode = (_currentColorBlindMode + 1) % _colorBlindModes.size();
                const auto mode = _colorBlindModes.at(_currentColorBlindMode);
                _renderer->setColorBlindMode(mode);

                std::string label;
                switch (mode) {
                    case Graphics::ColorBlindMode::NONE: label = "NORMAL"; break;
                    case Graphics::ColorBlindMode::DEUTERANOPIA: label = "DEUTER"; break;
                    case Graphics::ColorBlindMode::PROTANOPIA: label = "PROTAN"; break;
                    case Graphics::ColorBlindMode::TRITANOPIA: label = "TRITAN"; break;
                    default: label = "UNKNOWN"; break;
                }
                _colorBlindMode->setLabel(label);
                _colorBlindNext->reset();
                return;
            }

            if (_resolutionNext->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _currentResolution = (_currentResolution + 1) % _resolutions.size();
                _resolutionChanged = true;
                const auto &res = _resolutions.at(_currentResolution);
                _resolution->setLabel(std::to_string(res.width) + "x" + std::to_string(res.height));
                _resolutionNext->reset();
                return;
            }
            if (_back->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _backRequested = true;
                _back->reset();
            }
        }
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
        if (_resolutions.empty())
            return {0, 0};
        if (_currentResolution >= _resolutions.size())
            return _resolutions.back();
        return _resolutions.at(_currentResolution);
    }

    void SettingsMenu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _audio->render();
        _colorBlindMode->render();
        _colorBlindNext->render();
        _resolution->render();
        _resolutionNext->render();
        _back->render();
    }
} // namespace Engine