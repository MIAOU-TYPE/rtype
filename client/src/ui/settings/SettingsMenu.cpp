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
        const auto preset = Utils::SettingsConfig::getInstance().getCurrentPreset();
        const auto presetName = Utils::SettingsConfig::getPresetName(preset);
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
        const auto &config = Utils::SettingsConfig::getInstance();
        _rebindUp = std::make_unique<UI::UIButton>(
            _renderer, UI::ButtonSize::Small, Utils::SettingsConfig::keyToString(config.getKey(BindAction::Up)));
        _rebindDown = std::make_unique<UI::UIButton>(
            _renderer, UI::ButtonSize::Small, Utils::SettingsConfig::keyToString(config.getKey(BindAction::Down)));
        _rebindLeft = std::make_unique<UI::UIButton>(
            _renderer, UI::ButtonSize::Small, Utils::SettingsConfig::keyToString(config.getKey(BindAction::Left)));
        _rebindRight = std::make_unique<UI::UIButton>(
            _renderer, UI::ButtonSize::Small, Utils::SettingsConfig::keyToString(config.getKey(BindAction::Right)));
        _rebindShoot = std::make_unique<UI::UIButton>(
            _renderer, UI::ButtonSize::Small, Utils::SettingsConfig::keyToString(config.getKey(BindAction::Shoot)));

        _rebindUpLabel = _renderer->texts()->createText(16, {255, 255, 255, 255});
        _rebindUpLabel->setString("UP");
        _rebindDownLabel = _renderer->texts()->createText(16, {255, 255, 255, 255});
        _rebindDownLabel->setString("DOWN");
        _rebindLeftLabel = _renderer->texts()->createText(16, {255, 255, 255, 255});
        _rebindLeftLabel->setString("LEFT");
        _rebindRightLabel = _renderer->texts()->createText(16, {255, 255, 255, 255});
        _rebindRightLabel->setString("RIGHT");
        _rebindShootLabel = _renderer->texts()->createText(16, {255, 255, 255, 255});
        _rebindShootLabel->setString("SHOOT");

        _rebindButtonMap[RebindState::Up] = _rebindUp.get();
        _rebindButtonMap[RebindState::Down] = _rebindDown.get();
        _rebindButtonMap[RebindState::Left] = _rebindLeft.get();
        _rebindButtonMap[RebindState::Right] = _rebindRight.get();
        _rebindButtonMap[RebindState::Shoot] = _rebindShoot.get();
    }

    void SettingsMenu::onEnter()
    {
        _backRequested = false;
        _resolutionChanged = false;
        _controlsChanged = false;
        _errorState = RebindState::None;
        _errorFrameCount = 0;
        resetButtons(_colorBlindMode.get(), _colorBlindNext.get(), _resolution.get(), _resolutionNext.get(),
            _controls.get(), _controlsNext.get(), _back.get(), _musicVolLabel.get(), _musicVolUp.get(),
            _musicVolDown.get(), _sfxVolLabel.get(), _sfxVolUp.get(), _sfxVolDown.get(), _muteMusic.get(),
            _muteSFX.get(), _rebindUp.get(), _rebindDown.get(), _rebindLeft.get(), _rebindRight.get(),
            _rebindShoot.get());

        const auto &config = Utils::SettingsConfig::getInstance();
        _musicVolume = config.getMusicVolume();
        _sfxVolume = config.getSfxVolume();
        _musicMuted = config.isMusicMuted();
        _sfxMuted = config.isSfxMuted();
        _currentResolution = 0;
        const auto res = config.getResolution();
        for (std::size_t i = 0; i < _resolutions.size(); ++i) {
            if (_resolutions[i].width == res.width && _resolutions[i].height == res.height) {
                _currentResolution = i;
                break;
            }
        }
        _currentColorBlindMode = config.getColorBlindMode();

        if (_musicRegistry) {
            _musicRegistry->setMusicVolume(_musicMuted ? 0.f : static_cast<float>(_musicVolume));
            if (_musicMuted)
                _musicRegistry->setVolumeBeforeMute(static_cast<float>(_musicVolume));
        }
        if (_soundRegistry) {
            _soundRegistry->setSoundVolume(_sfxMuted ? 0.f : static_cast<float>(_sfxVolume));
            if (_sfxMuted)
                _soundRegistry->setVolumeBeforeMute(static_cast<float>(_sfxVolume));
        }
        _renderer->setColorBlindMode(_currentColorBlindMode);

        _musicVolLabel->setLabel(std::to_string(_musicVolume));
        _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
        _muteMusic->setLabel(_musicMuted ? "ON MUSIC" : "OFF MUSIC");
        _muteSFX->setLabel(_sfxMuted ? "ON SFX" : "OFF SFX");
        std::string label;
        switch (_currentColorBlindMode) {
            case Graphics::ColorBlindMode::NONE: label = "NORMAL"; break;
            case Graphics::ColorBlindMode::DEUTERANOPIA: label = "DEUTER"; break;
            case Graphics::ColorBlindMode::PROTANOPIA: label = "PROTAN"; break;
            case Graphics::ColorBlindMode::TRITANOPIA: label = "TRITAN"; break;
            default: label = "UNKNOWN"; break;
        }
        _colorBlindMode->setLabel(label);
        const auto &currentRes = _resolutions.at(_currentResolution);
        _resolution->setLabel(std::to_string(currentRes.width) + "x" + std::to_string(currentRes.height));
        _controls->setLabel(Utils::SettingsConfig::getPresetName(config.getCurrentPreset()));
        updateRebindLabels();
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
        placeCentered(*_rebindUp, rightColX - w * 0.1f, videoYStart + 3 * spacingY);
        placeCentered(*_rebindDown, rightColX, videoYStart + 3 * spacingY);
        placeCentered(*_rebindLeft, rightColX + w * 0.1f, videoYStart + 3 * spacingY);
        placeCentered(*_rebindRight, rightColX + w * 0.2f, videoYStart + 3 * spacingY);
        placeCentered(*_rebindShoot, rightColX, videoYStart + 4 * spacingY);

        const float labelOffsetY = h * 0.06f;
        _rebindUpLabel->setPosition(
            rightColX - w * 0.1f - _rebindUpLabel->getWidth() * 0.5f, videoYStart + 3 * spacingY + labelOffsetY);
        _rebindDownLabel->setPosition(
            rightColX - _rebindDownLabel->getWidth() * 0.5f, videoYStart + 3 * spacingY + labelOffsetY);
        _rebindLeftLabel->setPosition(
            rightColX + w * 0.1f - _rebindLeftLabel->getWidth() * 0.5f, videoYStart + 3 * spacingY + labelOffsetY);
        _rebindRightLabel->setPosition(
            rightColX + w * 0.2f - _rebindRightLabel->getWidth() * 0.5f, videoYStart + 3 * spacingY + labelOffsetY);
        _rebindShootLabel->setPosition(
            rightColX - _rebindShootLabel->getWidth() * 0.5f, videoYStart + 4 * spacingY + labelOffsetY);

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

        if (_errorState != RebindState::None) {
            _errorFrameCount++;
            if (_errorFrameCount >= ERROR_DISPLAY_FRAMES) {
                UI::UIButton *button = _rebindButtonMap[_errorState];
                if (button)
                    button->setLabel("...");
                _errorState = RebindState::None;
                _errorFrameCount = 0;
            }
        }

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
        if (_rebindState != RebindState::None) {
            auto &config = Utils::SettingsConfig::getInstance();
            if (frame.key == Key::Escape) {
                BindAction action = static_cast<BindAction>(_rebindState);
                _rebindButtonMap[_rebindState]->setLabel(Utils::SettingsConfig::keyToString(config.getKey(action)));
                _rebindState = RebindState::None;
                return;
            }
            Engine::Key currentKey = Engine::Key::Unknown;
            switch (_rebindState) {
                case RebindState::Up: currentKey = config.getKey(BindAction::Up); break;
                case RebindState::Down: currentKey = config.getKey(BindAction::Down); break;
                case RebindState::Left: currentKey = config.getKey(BindAction::Left); break;
                case RebindState::Right: currentKey = config.getKey(BindAction::Right); break;
                case RebindState::Shoot: currentKey = config.getKey(BindAction::Shoot); break;
                default: break;
            }

            if (config.isKeyAlreadyAssigned(frame.key, currentKey)) {
                _errorState = _rebindState;
                _errorFrameCount = 0;
                UI::UIButton *button = _rebindButtonMap[_rebindState];
                if (button)
                    button->setLabel("/!\\");
                return;
            }
            switch (_rebindState) {
                case RebindState::Up: config.setKey(BindAction::Up, frame.key); break;
                case RebindState::Down: config.setKey(BindAction::Down, frame.key); break;
                case RebindState::Left: config.setKey(BindAction::Left, frame.key); break;
                case RebindState::Right: config.setKey(BindAction::Right, frame.key); break;
                case RebindState::Shoot: config.setKey(BindAction::Shoot, frame.key); break;
                default: break;
            }
            _rebindButtonMap[_rebindState]->setLabel(Utils::SettingsConfig::keyToString(frame.key));
            _rebindState = RebindState::None;
            _controlsChanged = true;
            return;
        }

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

    void SettingsMenu::handleMousePressed(const InputFrame &frame) const
    {
        pressButtons(frame.mouseX, frame.mouseY, _colorBlindMode.get(), _colorBlindNext.get(), _resolution.get(),
            _resolutionNext.get(), _controls.get(), _controlsNext.get(), _back.get(), _musicVolUp.get(),
            _musicVolDown.get(), _sfxVolUp.get(), _sfxVolDown.get(), _muteMusic.get(), _muteSFX.get(), _rebindUp.get(),
            _rebindDown.get(), _rebindLeft.get(), _rebindRight.get(), _rebindShoot.get());
    }

    void SettingsMenu::handleMouseReleased(const InputFrame &frame)
    {
        if (handleVideoReleased(frame.mouseX, frame.mouseY))
            return;
        if (handleControlsReleased(frame.mouseX, frame.mouseY))
            return;
        if (handleAudioReleased(frame.mouseX, frame.mouseY))
            return;
        if (handleRebindReleased(frame.mouseX, frame.mouseY))
            return;
        (void) handleNavigationReleased(frame.mouseX, frame.mouseY);
    }

    bool SettingsMenu::handleVideoReleased(const float mx, const float my)
    {
        auto &config = Utils::SettingsConfig::getInstance();
        if (_colorBlindNext->onMouseReleased(mx, my)) {
            _currentColorBlindMode = nextMode(_currentColorBlindMode);
            _renderer->setColorBlindMode(_currentColorBlindMode);
            config.setColorBlindMode(_currentColorBlindMode);
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
            config.setResolution(_resolutions.at(_currentResolution));
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
        auto &config = Utils::SettingsConfig::getInstance();
        const auto current = config.getCurrentPreset();
        Utils::KeyPreset next;
        switch (current) {
            case Utils::KeyPreset::Arrows: next = Utils::KeyPreset::ZQSD; break;
            case Utils::KeyPreset::ZQSD: next = Utils::KeyPreset::Custom; break;
            case Utils::KeyPreset::Custom: next = Utils::KeyPreset::Arrows; break;
            default: next = Utils::KeyPreset::Arrows; break;
        }
        config.setPreset(next);
        _controls->setLabel(Utils::SettingsConfig::getPresetName(next));
        updateRebindLabels();
        _controlsChanged = true;
        _controlsNext->reset();
        return true;
    }

    bool SettingsMenu::handleAudioReleased(const float mx, const float my)
    {
        auto &config = Utils::SettingsConfig::getInstance();
        if (_musicVolUp->onMouseReleased(mx, my)) {
            _musicVolume = std::min<size_t>(100, _musicVolume + 10);
            _musicVolLabel->setLabel(std::to_string(_musicVolume));
            config.setMusicVolume(_musicVolume);
            applyMusicVolumeChange(_musicVolume, _musicMuted);
            _musicVolUp->reset();
            return true;
        }
        if (_musicVolDown->onMouseReleased(mx, my)) {
            _musicVolume = (_musicVolume >= 10) ? _musicVolume - 10 : 0;
            _musicVolLabel->setLabel(std::to_string(_musicVolume));
            config.setMusicVolume(_musicVolume);
            applyMusicVolumeChange(_musicVolume, _musicMuted);
            _musicVolDown->reset();
            return true;
        }
        if (_sfxVolUp->onMouseReleased(mx, my)) {
            _sfxVolume = std::min<size_t>(100, _sfxVolume + 10);
            _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
            config.setSfxVolume(_sfxVolume);
            applySoundVolumeChange(_sfxVolume, _sfxMuted);
            _sfxVolUp->reset();
            return true;
        }
        if (_sfxVolDown->onMouseReleased(mx, my)) {
            _sfxVolume = (_sfxVolume >= 10) ? _sfxVolume - 10 : 0;
            _sfxVolLabel->setLabel(std::to_string(_sfxVolume));
            config.setSfxVolume(_sfxVolume);
            applySoundVolumeChange(_sfxVolume, _sfxMuted);
            _sfxVolDown->reset();
            return true;
        }
        if (_muteMusic->onMouseReleased(mx, my)) {
            _musicMuted = !_musicMuted;
            _muteMusic->setLabel(_musicMuted ? "ON MUSIC" : "OFF MUSIC");
            config.setMusicMuted(_musicMuted);
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
            config.setSfxMuted(_sfxMuted);
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

    bool SettingsMenu::handleRebindReleased(const float mx, const float my)
    {
        auto &config = Utils::SettingsConfig::getInstance();
        if (config.getCurrentPreset() != Utils::KeyPreset::Custom)
            return false;

        for (const auto &[state, button] : _rebindButtonMap) {
            if (button->onMouseReleased(mx, my)) {
                _rebindState = state;
                button->setLabel("...");
                button->reset();
                return true;
            }
        }
        return false;
    }

    void SettingsMenu::updateRebindLabels()
    {
        auto &config = Utils::SettingsConfig::getInstance();
        const auto keys = config.getMovementKeys();
        _rebindUp->setLabel(Utils::SettingsConfig::keyToString(keys.up));
        _rebindDown->setLabel(Utils::SettingsConfig::keyToString(keys.down));
        _rebindLeft->setLabel(Utils::SettingsConfig::keyToString(keys.left));
        _rebindRight->setLabel(Utils::SettingsConfig::keyToString(keys.right));
        _rebindShoot->setLabel(Utils::SettingsConfig::keyToString(keys.shoot));
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

        if (Utils::SettingsConfig::getInstance().getCurrentPreset() == Utils::KeyPreset::Custom) {
            _rebindUp->render();
            _rebindDown->render();
            _rebindLeft->render();
            _rebindRight->render();
            _rebindShoot->render();
            _renderer->draw(*_rebindUpLabel);
            _renderer->draw(*_rebindDownLabel);
            _renderer->draw(*_rebindLeftLabel);
            _renderer->draw(*_rebindRightLabel);
            _renderer->draw(*_rebindShootLabel);
        }
    }
} // namespace Engine