/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"
#include <iostream>

namespace
{
    Graphics::ColorBlindMode nextMode(Graphics::ColorBlindMode m)
    {
        switch (m) {
            case Graphics::ColorBlindMode::NONE: return Graphics::ColorBlindMode::DEUTERANOPIA;
            case Graphics::ColorBlindMode::DEUTERANOPIA: return Graphics::ColorBlindMode::PROTANOPIA;
            case Graphics::ColorBlindMode::PROTANOPIA: return Graphics::ColorBlindMode::TRITANOPIA;
            case Graphics::ColorBlindMode::TRITANOPIA: return Graphics::ColorBlindMode::NONE;
            default: return Graphics::ColorBlindMode::NONE;
        }
    }
} // namespace

namespace Engine
{
    SettingsMenu::SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer,
        std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry)
        : _renderer(renderer), _musicRegistry(std::move(musicRegistry)), _soundRegistry(std::move(soundRegistry))
    {
        const auto textures = renderer->textures();

        _backgroundTexture = textures->load("sprites/bg-preview.png");
        if (_backgroundTexture == Graphics::InvalidTexture)
            throw SettingsMenuError("SettingsMenu: failed to load background texture");

        _backgroundCmd.textureId = _backgroundTexture;

        _colorBlindMode = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "NORMAL");
        _colorBlindNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _resolution = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "1280x720");
        _resolutionNext = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _back = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "BACK");
        _musicVolLabel = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "50");
        _musicVolUp = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _musicVolDown = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "-");
        _sfxVolLabel = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "50");
        _sfxVolUp = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "+");
        _sfxVolDown = std::make_unique<UIButton>(_renderer, ButtonSize::Small, "-");
        _muteMusic = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "OFF MUSIC");
        _muteSFX = std::make_unique<UIButton>(_renderer, ButtonSize::Large, "OFF SFX");
    }

    void SettingsMenu::onEnter()
    {
        _backRequested = false;
        _resolutionChanged = false;

        _colorBlindMode->reset();
        _colorBlindNext->reset();
        _resolution->reset();
        _resolutionNext->reset();
        _back->reset();
        _musicVolLabel->reset();
        _musicVolUp->reset();
        _musicVolDown->reset();
        _sfxVolLabel->reset();
        _sfxVolUp->reset();
        _sfxVolDown->reset();
        _muteMusic->reset();
        _muteSFX->reset();

        _musicVolLabel->setLabel(std::to_string(_musicVolume));
        _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
        _muteMusic->setLabel(_musicMuted ? "ON MUSIC" : "OFF MUSIC");
        _muteSFX->setLabel(_sfxMuted ? "ON SFX" : "OFF SFX");

        _currentColorBlindMode = _renderer->getColorBlindMode();

        std::string label;
        switch (_currentColorBlindMode) {
            case Graphics::ColorBlindMode::NONE: label = "NORMAL"; break;
            case Graphics::ColorBlindMode::DEUTERANOPIA: label = "DEUTER"; break;
            case Graphics::ColorBlindMode::PROTANOPIA: label = "PROTAN"; break;
            case Graphics::ColorBlindMode::TRITANOPIA: label = "TRITAN"; break;
            default: label = "UNKNOWN"; break;
        }
        _colorBlindMode->setLabel(label);
        const auto &res = _resolutions.at(_currentResolution);
        _resolution->setLabel(std::to_string(res.width) + "x" + std::to_string(res.height));

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
        const float audioX = w * 0.25f;
        const float audioYStart = h * 0.25f;
        const float spacingY = h * 0.12f;
        const float labelMargin = w * 0.02f;
        const float musicLabelHalfWidth = _musicVolLabel->bounds().w * 0.5f;
        const float buttonHalfWidth = _musicVolDown->bounds().w * 0.5f;
        const float sfxLabelHalfWidth = _sfxVolLabel->bounds().w * 0.5f;

        _backgroundCmd.frame = {0, 0, static_cast<int>(texSize.width), static_cast<int>(texSize.height)};
        _backgroundCmd.position = {0.f, 0.f};
        _backgroundCmd.scale = {w / static_cast<float>(texSize.width), h / static_cast<float>(texSize.height)};

        _colorBlindMode->setPosition(cx - 400.f * scale, cy + 40.f * scale);
        _colorBlindNext->setPosition(cx + (150.f + _colorBlindMode->bounds().w + 250.f) * scale, cy + 40.f * scale);
        _resolution->setPosition(cx - 400.f * scale, cy - 260.f * scale);
        _resolutionNext->setPosition(cx + (150.f + _resolution->bounds().w + 250.f) * scale, cy - 260.f * scale);
        _back->setPosition(cx - _back->bounds().w * 0.5f, h * 0.8f);

        _musicVolLabel->setPosition(audioX - musicLabelHalfWidth, audioYStart);
        _musicVolDown->setPosition(
            audioX - (musicLabelHalfWidth + labelMargin + buttonHalfWidth) - buttonHalfWidth, audioYStart);
        _musicVolUp->setPosition(
            audioX + (musicLabelHalfWidth + labelMargin + buttonHalfWidth) - buttonHalfWidth, audioYStart);

        _sfxVolLabel->setPosition(audioX - sfxLabelHalfWidth, audioYStart + spacingY);
        _sfxVolDown->setPosition(
            audioX - (sfxLabelHalfWidth + labelMargin + buttonHalfWidth) - buttonHalfWidth, audioYStart + spacingY);
        _sfxVolUp->setPosition(
            audioX + (sfxLabelHalfWidth + labelMargin + buttonHalfWidth) - buttonHalfWidth, audioYStart + spacingY);

        _muteMusic->setPosition(audioX - _muteMusic->bounds().w * 0.5f, audioYStart + 2 * spacingY);
        _muteSFX->setPosition(audioX - _muteSFX->bounds().w * 0.5f, audioYStart + 3 * spacingY);
    }

    void SettingsMenu::update(const InputFrame &frame)
    {
        handleInput(frame);
        _colorBlindMode->update(frame.mouseX, frame.mouseY);
        _colorBlindNext->update(frame.mouseX, frame.mouseY);
        _resolution->update(frame.mouseX, frame.mouseY);
        _resolutionNext->update(frame.mouseX, frame.mouseY);
        _back->update(frame.mouseX, frame.mouseY);
        _musicVolLabel->update(frame.mouseX, frame.mouseY);
        _musicVolUp->update(frame.mouseX, frame.mouseY);
        _musicVolDown->update(frame.mouseX, frame.mouseY);
        _sfxVolLabel->update(frame.mouseX, frame.mouseY);
        _sfxVolUp->update(frame.mouseX, frame.mouseY);
        _sfxVolDown->update(frame.mouseX, frame.mouseY);
        _muteMusic->update(frame.mouseX, frame.mouseY);
        _muteSFX->update(frame.mouseX, frame.mouseY);
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
            _colorBlindMode->onMousePressed(frame.mouseX, frame.mouseY);
            _colorBlindNext->onMousePressed(frame.mouseX, frame.mouseY);
            _resolution->onMousePressed(frame.mouseX, frame.mouseY);
            _resolutionNext->onMousePressed(frame.mouseX, frame.mouseY);
            _back->onMousePressed(frame.mouseX, frame.mouseY);
            _musicVolUp->onMousePressed(frame.mouseX, frame.mouseY);
            _musicVolDown->onMousePressed(frame.mouseX, frame.mouseY);
            _sfxVolUp->onMousePressed(frame.mouseX, frame.mouseY);
            _sfxVolDown->onMousePressed(frame.mouseX, frame.mouseY);
            _muteMusic->onMousePressed(frame.mouseX, frame.mouseY);
            _muteSFX->onMousePressed(frame.mouseX, frame.mouseY);
        }
        if (frame.mouseReleased) {
            if (_colorBlindNext->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _currentColorBlindMode = nextMode(_currentColorBlindMode);
                _renderer->setColorBlindMode(_currentColorBlindMode);

                std::string label;
                switch (_currentColorBlindMode) {
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
                return;
            }
            if (_musicVolUp->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _musicVolume = std::min(100, _musicVolume + 10);
                _musicVolLabel->setLabel(std::to_string(_musicVolume));
                if (_musicRegistry && !_musicMuted)
                    _musicRegistry->setMusicVolume(static_cast<float>(_musicVolume));
                _musicVolUp->reset();
                return;
            }
            if (_musicVolDown->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _musicVolume = std::max(0, _musicVolume - 10);
                _musicVolLabel->setLabel(std::to_string(_musicVolume));
                if (_musicRegistry && !_musicMuted)
                    _musicRegistry->setMusicVolume(static_cast<float>(_musicVolume));
                _musicVolDown->reset();
                return;
            }
            if (_sfxVolUp->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _sfxVolume = std::min(100, _sfxVolume + 10);
                _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
                if (_soundRegistry && !_sfxMuted)
                    _soundRegistry->setSoundVolume(static_cast<float>(_sfxVolume));
                _sfxVolUp->reset();
                return;
            }
            if (_sfxVolDown->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _sfxVolume = std::max(0, _sfxVolume - 10);
                _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
                if (_soundRegistry && !_sfxMuted)
                    _soundRegistry->setSoundVolume(static_cast<float>(_sfxVolume));
                _sfxVolDown->reset();
                return;
            }
            if (_muteMusic->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _musicMuted = !_musicMuted;
                _muteMusic->setLabel(_musicMuted ? "ON MUSIC" : "OFF MUSIC");
                if (_musicRegistry)
                    _musicRegistry->setMusicVolume(_musicMuted ? 0.f : static_cast<float>(_musicVolume));
                _muteMusic->reset();
                return;
            }
            if (_muteSFX->onMouseReleased(frame.mouseX, frame.mouseY)) {
                _sfxMuted = !_sfxMuted;
                _muteSFX->setLabel(_sfxMuted ? "ON SFX" : "OFF SFX");
                if (_soundRegistry)
                    _soundRegistry->setSoundVolume(_sfxMuted ? 0.f : static_cast<float>(_sfxVolume));
                _muteSFX->reset();
                return;
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
        _colorBlindMode->render();
        _colorBlindNext->render();
        _resolution->render();
        _resolutionNext->render();
        _back->render();
        _musicVolLabel->render();
        _musicVolUp->render();
        _musicVolDown->render();
        _sfxVolLabel->render();
        _sfxVolUp->render();
        _sfxVolDown->render();
        _muteMusic->render();
        _muteSFX->render();
    }
} // namespace Engine