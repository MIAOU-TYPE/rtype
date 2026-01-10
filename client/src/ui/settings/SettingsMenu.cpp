/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"

namespace
{
    Graphics::ColorBlindMode nextMode(Graphics::ColorBlindMode m)
    {
        switch (m) {
            case Graphics::ColorBlindMode::NONE: return Graphics::ColorBlindMode::DEUTERANOPIA;
            case Graphics::ColorBlindMode::DEUTERANOPIA: return Graphics::ColorBlindMode::PROTANOPIA;
            case Graphics::ColorBlindMode::PROTANOPIA: return Graphics::ColorBlindMode::TRITANOPIA;
            default: return Graphics::ColorBlindMode::NONE;
        }
    }
} // namespace

namespace Engine
{
    SettingsMenu::SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer,
        std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry)
        : AMenu(renderer), _musicRegistry(std::move(musicRegistry)), _soundRegistry(std::move(soundRegistry))
    {
        loadBackground("sprites/bg-preview.png");
        _colorBlindMode = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "NORMAL");
        _colorBlindNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _resolution = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "1280x720");
        _resolutionNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        const auto preset = Utils::InputConfig::getInstance().getCurrentPreset();
        const auto presetName = Utils::InputConfig::getPresetName(preset);
        _controls = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, presetName);
        _controlsNext = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");
        _musicVolLabel = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "50");
        _musicVolUp = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _musicVolDown = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "-");
        _sfxVolLabel = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "50");
        _sfxVolUp = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _sfxVolDown = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "-");
        _muteMusic = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "OFF MUSIC");
        _muteSFX = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "OFF SFX");
    }

    void SettingsMenu::onEnter()
    {
        _backRequested = false;
        _resolutionChanged = false;
        _controlsChanged = false;
        resetButtons(_colorBlindMode.get(), _colorBlindNext.get(), _resolution.get(), _resolutionNext.get(),
            _controls.get(), _controlsNext.get(), _back.get(), _musicVolLabel.get(), _musicVolUp.get(),
            _musicVolDown.get(), _sfxVolLabel.get(), _sfxVolUp.get(), _sfxVolDown.get(), _muteMusic.get(),
            _muteSFX.get());
        if (_musicRegistry) {
            const float musicVol = _musicRegistry->getMusicVolume();
            const float volumeBeforeMute = _musicRegistry->getVolumeBeforeMute();
            if (musicVol < 0.01f && volumeBeforeMute > 0.01f) {
                _musicMuted = true;
                _musicVolume = static_cast<size_t>(volumeBeforeMute);
                _musicVolumeBeforeMute = static_cast<size_t>(volumeBeforeMute);
            } else {
                _musicMuted = false;
                _musicVolume = static_cast<size_t>(musicVol);
                _musicVolumeBeforeMute = _musicVolume;
            }
        }
        if (_soundRegistry) {
            const float sfxVol = _soundRegistry->getSoundVolume();
            const float volumeBeforeMute = _soundRegistry->getVolumeBeforeMute();
            if (sfxVol < 0.01f && volumeBeforeMute > 0.01f) {
                _sfxMuted = true;
                _sfxVolume = static_cast<size_t>(volumeBeforeMute);
                _sfxVolumeBeforeMute = static_cast<size_t>(volumeBeforeMute);
            } else {
                _sfxMuted = false;
                _sfxVolume = static_cast<size_t>(sfxVol);
                _sfxVolumeBeforeMute = _sfxVolume;
            }
        }
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
        const auto vp = viewportF();
        const float w = vp.w;
        const float h = vp.h;
        const float cx = vp.cx;
        const float leftColX = w * 0.25f;
        const float rightColX = w * 0.75f;
        const float audioYStart = h * 0.2f;
        const float videoYStart = h * 0.3f;
        const float spacingY = h * 0.12f;
        const float labelMargin = w * 0.02f;
        const float rightMargin = w * 0.05f;
        const float videoButtonSpacing = w * 0.05f;

        layoutBackground();
        layoutStepper(*_musicVolLabel, *_musicVolDown, *_musicVolUp, leftColX, audioYStart, labelMargin);
        layoutStepper(*_sfxVolLabel, *_sfxVolDown, *_sfxVolUp, leftColX, audioYStart + spacingY, labelMargin);
        placeCentered(*_muteMusic, leftColX, audioYStart + 2 * spacingY);
        placeCentered(*_muteSFX, leftColX, audioYStart + 3 * spacingY);
        layoutChoiceWithNext(*_colorBlindMode, *_colorBlindNext, rightColX, videoYStart, labelMargin,
            videoButtonSpacing, w, rightMargin);
        layoutChoiceWithNext(*_resolution, *_resolutionNext, rightColX, videoYStart + spacingY, labelMargin,
            videoButtonSpacing, w, rightMargin);
        layoutChoiceWithNext(*_controls, *_controlsNext, rightColX, videoYStart + 2 * spacingY, labelMargin,
            videoButtonSpacing, w, rightMargin);
        placeCentered(*_back, cx, h * 0.8f);
    }

    void SettingsMenu::applyMusicVolumeChange(const size_t volume, const bool isMuted) noexcept
    {
        if (!_musicRegistry)
            return;
        if (isMuted) {
            _musicVolumeBeforeMute = volume;
            _musicRegistry->setVolumeBeforeMute(static_cast<float>(volume));
        } else {
            _musicRegistry->setMusicVolume(static_cast<float>(volume));
        }
    }

    void SettingsMenu::applySoundVolumeChange(const size_t volume, const bool isMuted) noexcept
    {
        if (!_soundRegistry)
            return;
        if (isMuted) {
            _sfxVolumeBeforeMute = volume;
            _soundRegistry->setVolumeBeforeMute(static_cast<float>(volume));
        } else {
            _soundRegistry->setSoundVolume(static_cast<float>(volume));
        }
    }

    void SettingsMenu::update(const InputFrame &frame)
    {
        handleInput(frame);
        updateButtons(frame.mouseX, frame.mouseY, _colorBlindMode.get(), _colorBlindNext.get(), _resolution.get(),
            _resolutionNext.get(), _controls.get(), _controlsNext.get(), _back.get(), _musicVolLabel.get(),
            _musicVolUp.get(), _musicVolDown.get(), _sfxVolLabel.get(), _sfxVolUp.get(), _sfxVolDown.get(),
            _muteMusic.get(), _muteSFX.get());
    }

    void SettingsMenu::handleInput(const InputFrame &frame)
    {
        if (frame.keyPressed)
            handleKeyPressed(frame);
        if (frame.mousePressed)
            handleMousePressed(frame);
        if (frame.mouseReleased)
            handleMouseReleased(frame);
    }

    void SettingsMenu::handleKeyPressed(const InputFrame &frame)
    {
        switch (frame.key) {
            case Key::B:
                _backRequested = true;
                break;
            case Key::Backspace: {
                _currentResolution = (_currentResolution + 1) % _resolutions.size();
                const auto &res = _resolutions.at(_currentResolution);
                _resolution->setLabel(std::to_string(res.width) + "x" + std::to_string(res.height));
                _resolutionChanged = true;
                break;
            }
            default:
                break;
        }
    }

    void SettingsMenu::handleMousePressed(const InputFrame &frame) const
    {
        pressButtons(frame.mouseX, frame.mouseY,
            _colorBlindMode.get(), _colorBlindNext.get(),
            _resolution.get(), _resolutionNext.get(),
            _controls.get(), _controlsNext.get(),
            _back.get(),
            _musicVolUp.get(), _musicVolDown.get(),
            _sfxVolUp.get(), _sfxVolDown.get(),
            _muteMusic.get(), _muteSFX.get());
    }

    void SettingsMenu::handleMouseReleased(const InputFrame &frame)
    {
        if (handleVideoReleased(frame.mouseX, frame.mouseY)) return;
        if (handleControlsReleased(frame.mouseX, frame.mouseY)) return;
        if (handleAudioReleased(frame.mouseX, frame.mouseY)) return;
        handleNavigationReleased(frame.mouseX, frame.mouseY);
    }

    bool SettingsMenu::handleVideoReleased(const float mx, const float my)
    {
        if (_colorBlindNext->onMouseReleased(mx, my)) {
            _currentColorBlindMode = nextMode(_currentColorBlindMode);
            _renderer->setColorBlindMode(_currentColorBlindMode);
            static const std::unordered_map<Graphics::ColorBlindMode, std::string> labels = {
                {Graphics::ColorBlindMode::NONE, "NORMAL"},
                {Graphics::ColorBlindMode::DEUTERANOPIA, "DEUTER"},
                {Graphics::ColorBlindMode::PROTANOPIA, "PROTAN"},
                {Graphics::ColorBlindMode::TRITANOPIA, "TRITAN"},
            };
            _colorBlindMode->setLabel(labels.at(_currentColorBlindMode));
            _colorBlindNext->reset();
            return true;
        }
        if (_resolutionNext->onMouseReleased(mx, my)) {
            _currentResolution = (_currentResolution + 1) % _resolutions.size();
            _resolutionChanged = true;
            const auto &[width, height] = _resolutions.at(_currentResolution);
            _resolution->setLabel(std::to_string(width) + "x" + std::to_string(height));
            return true;
        }
        return false;
    }

    bool SettingsMenu::handleControlsReleased(const float mx, const float my)
    {
        if (!_controlsNext->onMouseReleased(mx, my))
            return false;
        auto &config = Utils::InputConfig::getInstance();
        const auto current = config.getCurrentPreset();
        const auto next =
            current == Utils::KeyPreset::Arrows
                ? Utils::KeyPreset::ZQSD
                : Utils::KeyPreset::Arrows;
        config.setPreset(next);
        _controls->setLabel(Utils::InputConfig::getPresetName(next));
        _controlsChanged = true;
        _controlsNext->reset();
        return true;
    }

    bool SettingsMenu::handleAudioReleased(const float mx, const float my)
    {
        if (_musicVolUp->onMouseReleased(mx, my)) {
            _musicVolume = std::min<size_t>(100, _musicVolume + 10);
            _musicVolLabel->setLabel(std::to_string(_musicVolume));
            applyMusicVolumeChange(_musicVolume, _musicMuted);
            _musicVolUp->reset();
            return true;
        }
        if (_musicVolDown->onMouseReleased(mx, my)) {
            _musicVolume = (_musicVolume >= 10) ? _musicVolume - 10 : 0;
            _musicVolLabel->setLabel(std::to_string(_musicVolume));
            applyMusicVolumeChange(_musicVolume, _musicMuted);
            _musicVolDown->reset();
            return true;
        }
        if (_sfxVolUp->onMouseReleased(mx, my)) {
            _sfxVolume = std::min<size_t>(100, _sfxVolume + 10);
            _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
            applySoundVolumeChange(_sfxVolume, _sfxMuted);
            _sfxVolUp->reset();
            return true;
        }
        if (_sfxVolDown->onMouseReleased(mx, my)) {
            _sfxVolume = (_sfxVolume >= 10) ? _sfxVolume - 10 : 0;
            _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
            applySoundVolumeChange(_sfxVolume, _sfxMuted);
            _sfxVolDown->reset();
            return true;
        }
        if (_muteMusic->onMouseReleased(mx, my)) {
            _musicMuted = !_musicMuted;
            _muteMusic->setLabel(_musicMuted ? "ON MUSIC" : "OFF MUSIC");
            if (_musicRegistry) {
                _musicRegistry->setMusicVolume(_musicMuted ? 0.f : static_cast<float>(_musicVolume));
                if (_musicMuted)
                    _musicRegistry->setVolumeBeforeMute(static_cast<float>(_musicVolume));
            }
            _muteMusic->reset();
            return true;
        }
        if (_muteSFX->onMouseReleased(mx, my)) {
            _sfxMuted = !_sfxMuted;
            _muteSFX->setLabel(_sfxMuted ? "ON SFX" : "OFF SFX");
            if (_soundRegistry) {
                _soundRegistry->setSoundVolume(_sfxMuted ? 0.f : static_cast<float>(_sfxVolume));
                if (_sfxMuted)
                    _soundRegistry->setVolumeBeforeMute(static_cast<float>(_sfxVolume));
            }
            _muteSFX->reset();
            return true;
        }
        return false;
    }

    bool SettingsMenu::handleNavigationReleased(const float mx, const float my)
    {
        if (!_back->onMouseReleased(mx, my))
            return false;
        _backRequested = true;
        _back->reset();
        return true;
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
        renderBackground();
        _colorBlindMode->render();
        _colorBlindNext->render();
        _resolution->render();
        _resolutionNext->render();
        _controls->render();
        _controlsNext->render();
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