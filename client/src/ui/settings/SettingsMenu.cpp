/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"
#include <iostream>

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
        _resolution = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "1280x720");
        _resolutionNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        
        const auto preset = Utils::InputConfig::getInstance().getCurrentPreset();
        const auto presetName = Utils::InputConfig::getPresetName(preset);
        std::cout << "Current preset: " << presetName << std::endl;
        _controls = std::make_unique<UIButton>(_renderer, ButtonSize::Large, presetName);
        _controlsNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        
        _back = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "BACK");
    }

    void SettingsMenu::onEnter()
    {
        _backRequested = false;
        _resolutionChanged = false;
        _controlsChanged = false;

        _audio->reset();
        _resolution->reset();
        _resolutionNext->reset();
        _controls->reset();
        _controlsNext->reset();
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

        _audio->setPosition(cx - 400.f * scale, cy - 150.f * scale);
        _resolution->setPosition(cx + 150.f * scale, cy - 150.f * scale);
        _resolutionNext->setPosition(cx + (150.f + _resolution->bounds().w + 250.f) * scale, cy - 150.f * scale);
        
        _controls->setPosition(cx + 150.f * scale, cy - 40.f * scale);
        _controlsNext->setPosition(cx + (150.f + _controls->bounds().w + 250.f) * scale, cy - 40.f * scale);
        
        _back->setPosition(cx - _back->bounds().w * 0.5f, h * 0.8f);
    }

    void SettingsMenu::update(const InputFrame &frame)
    {
        handleInput(frame);
        _audio->update(frame.mouseX, frame.mouseY);
        _resolution->update(frame.mouseX, frame.mouseY);
        _resolutionNext->update(frame.mouseX, frame.mouseY);
        _controls->update(frame.mouseX, frame.mouseY);
        _controlsNext->update(frame.mouseX, frame.mouseY);
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
            _resolution->onMousePressed(frame.mouseX, frame.mouseY);
            _resolutionNext->onMousePressed(frame.mouseX, frame.mouseY);
            _controls->onMousePressed(frame.mouseX, frame.mouseY);
            _controlsNext->onMousePressed(frame.mouseX, frame.mouseY);
            _back->onMousePressed(frame.mouseX, frame.mouseY);
        }
        if (frame.mouseReleased) {
            (void) _audio->onMouseReleased(frame.mouseX, frame.mouseY);

            if (_resolutionNext->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _currentResolution = (_currentResolution + 1) % _resolutions.size();
                _resolutionChanged = true;
                const auto &res = _resolutions.at(_currentResolution);
                _resolution->setLabel(std::to_string(res.width) + "x" + std::to_string(res.height));
                _resolutionNext->reset();
                return;
            }
            
            if (_controlsNext->onMouseReleased(frame.mouseX, frame.mouseY)) {
                auto &config = Utils::InputConfig::getInstance();
                const auto current = config.getCurrentPreset();
                const auto next = (current == Utils::KeyPreset::Arrows) ? Utils::KeyPreset::ZQSD : Utils::KeyPreset::Arrows;
                config.setPreset(next);
                _controls->setLabel(Utils::InputConfig::getPresetName(next));
                _controlsChanged = true;
                _controlsNext->reset();
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

    bool SettingsMenu::controlsChanged() const noexcept
    {
        return _controlsChanged;
    }

    void SettingsMenu::render() const
    {
        _renderer->draw(_backgroundCmd);
        _audio->render();
        _resolution->render();
        _resolutionNext->render();
        _controls->render();
        _controlsNext->render();
        _back->render();
    }
} // namespace Engine