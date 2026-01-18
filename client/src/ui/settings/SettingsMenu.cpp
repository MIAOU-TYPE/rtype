/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"

namespace
{
    [[nodiscard]] Graphics::ColorBlindMode nextMode(Graphics::ColorBlindMode m)
    {
        switch (m) {
            case Graphics::ColorBlindMode::NONE: return Graphics::ColorBlindMode::DEUTERANOPIA;
            case Graphics::ColorBlindMode::DEUTERANOPIA: return Graphics::ColorBlindMode::PROTANOPIA;
            case Graphics::ColorBlindMode::PROTANOPIA: return Graphics::ColorBlindMode::TRITANOPIA;
            default: return Graphics::ColorBlindMode::NONE;
        }
    }

    [[nodiscard]] BindAction toBindAction(Engine::SettingsMenu::RebindState s)
    {
        using RS = Engine::SettingsMenu::RebindState;
        switch (s) {
            case RS::Up: return BindAction::Up;
            case RS::Down: return BindAction::Down;
            case RS::Left: return BindAction::Left;
            case RS::Right: return BindAction::Right;
            case RS::Shoot: return BindAction::Shoot;
            default: return BindAction::Up;
        }
    }

    void setCenteredText(Graphics::IText &t, const float cx, const float cy, const float approxH)
    {
        t.setPosition(cx - t.getWidth() * 0.5f, cy - approxH * 0.5f);
    }

    [[nodiscard]] std::string colorBlindLabel(const Graphics::ColorBlindMode mode)
    {
        switch (mode) {
            case Graphics::ColorBlindMode::NONE: return "NORMAL";
            case Graphics::ColorBlindMode::DEUTERANOPIA: return "DEUTER";
            case Graphics::ColorBlindMode::PROTANOPIA: return "PROTAN";
            case Graphics::ColorBlindMode::TRITANOPIA: return "TRITAN";
            default: return "UNKNOWN";
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
        loadPanel("sprites/popup.png", true);

        _title = _renderer->texts()->createText(46, {255, 255, 255, 255});
        _title->setString("SETTINGS");
        _musicVolValueText = _renderer->texts()->createText(36, {255, 255, 255, 255});
        _musicVolValueText->setString("50");
        _sfxVolValueText = _renderer->texts()->createText(36, {255, 255, 255, 255});
        _sfxVolValueText->setString("50");

        _musicVolUp = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _musicVolDown = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "-");
        _sfxVolUp = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "+");
        _sfxVolDown = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Small, "-");
        _muteMusic = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "OFF MUSIC");
        _muteSFX = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "OFF SOUNDS");
        _colorBlindCycle = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "NORMAL");
        _resolutionCycle = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "1280x720");
        _controlsCycle = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "ARROWS");
        _back = std::make_unique<UI::UIButton>(_renderer, UI::ButtonSize::Large, "BACK");

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
        _colorBlindTitleText = _renderer->texts()->createText(22, {200, 200, 200, 255});
        _colorBlindTitleText->setString("COLOR");
        _resolutionTitleText = _renderer->texts()->createText(22, {200, 200, 200, 255});
        _resolutionTitleText->setString("RESOLUTION");
        _controlsTitleText = _renderer->texts()->createText(22, {200, 200, 200, 255});
        _controlsTitleText->setString("CONTROL");

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
        resetButtons(_back.get(), _musicVolUp.get(), _musicVolDown.get(), _sfxVolUp.get(), _sfxVolDown.get(),
            _muteMusic.get(), _muteSFX.get(), _colorBlindCycle.get(), _resolutionCycle.get(), _controlsCycle.get(),
            _rebindUp.get(), _rebindDown.get(), _rebindLeft.get(), _rebindRight.get(), _rebindShoot.get());

        const auto &config = Utils::SettingsConfig::getInstance();
        _musicVolume = config.getMusicVolume();
        _sfxVolume = config.getSfxVolume();
        _musicMuted = config.isMusicMuted();
        _sfxMuted = config.isSfxMuted();
        _currentResolution = 0;
        const auto [width, height] = config.getResolution();
        for (size_t i = 0; i < _resolutions.size(); ++i) {
            if (_resolutions.at(i).width == width && _resolutions.at(i).height == height) {
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

        _musicVolValueText->setString(std::to_string(_musicVolume));
        _sfxVolValueText->setString(std::to_string(_sfxVolume));
        _muteMusic->setLabel(_musicMuted ? "ON MUSIC" : "OFF MUSIC");
        _muteSFX->setLabel(_sfxMuted ? "ON SFX" : "OFF SFX");

        _colorBlindCycle->setLabel(colorBlindLabel(_currentColorBlindMode));
        const auto &currentRes = _resolutions.at(_currentResolution);
        _resolutionCycle->setLabel(std::to_string(currentRes.width) + "x" + std::to_string(currentRes.height));
        _controlsCycle->setLabel(Utils::SettingsConfig::getPresetName(config.getCurrentPreset()));
        updateRebindLabels();
        layout();
    }

    void SettingsMenu::layout()
    {
        layoutBackground();
        layoutPanel(0.90f, 0.90f, 0.08f, 0.10f);
        const auto vp = viewportF();
        const float ui = std::clamp(std::min(vp.w / 1280.f, vp.h / 720.f), 0.65f, 1.0f);
        auto applyUI = [&](UI::UIButton *b) {
            if (b)
                b->setUIScale(ui, ui);
        };
        applyUI(_musicVolUp.get());
        applyUI(_musicVolDown.get());
        applyUI(_sfxVolUp.get());
        applyUI(_sfxVolDown.get());
        applyUI(_muteMusic.get());
        applyUI(_muteSFX.get());
        applyUI(_colorBlindCycle.get());
        applyUI(_resolutionCycle.get());
        applyUI(_controlsCycle.get());
        applyUI(_rebindUp.get());
        applyUI(_rebindDown.get());
        applyUI(_rebindLeft.get());
        applyUI(_rebindRight.get());
        applyUI(_rebindShoot.get());
        applyUI(_back.get());

        const auto inner = innerRect();
        const auto panel = panelRect();
        _title->setPosition(inner.cx() - _title->getWidth() * 0.5f, inner.y + inner.h * 0.05f - 90.f);

        const float insetX = panel.w * 0.08f;
        const float insetY = panel.h * 0.10f;
        const float cx0 = panel.x + insetX;
        const float cy0 = panel.y + insetY;
        const float cw = panel.w - insetX * 2.f;
        const float ch = panel.h - insetY * 2.f;

        setCenteredText(*_musicVolValueText, cx0 + cw * 0.25f, cy0 + ch * 0.10f + 25.f, 36.f);
        placeCentered(*_musicVolDown, (cx0 + cw * 0.25f) - cw * 0.12f, cy0 + ch * 0.10f + 25.f);
        placeCentered(*_musicVolUp, (cx0 + cw * 0.25f) + cw * 0.12f, cy0 + ch * 0.10f + 25.f);
        placeCentered(*_muteMusic, cx0 + cw * 0.25f, cy0 + ch * 0.36f - 25.f);

        setCenteredText(*_sfxVolValueText, cx0 + cw * 0.75f, cy0 + ch * 0.10f + 25.f, 36.f);
        placeCentered(*_sfxVolDown, (cx0 + cw * 0.75f) - cw * 0.12f, cy0 + ch * 0.10f + 25.f);
        placeCentered(*_sfxVolUp, (cx0 + cw * 0.75f) + cw * 0.12f, cy0 + ch * 0.10f + 25.f);
        placeCentered(*_muteSFX, cx0 + cw * 0.75f, cy0 + ch * 0.36f - 25.f);

        const float rebindRowY = cy0 + ch * 0.50f;
        const float step = cw * 0.12f;
        const float startX = inner.cx() - step * 2.f;

        placeCentered(*_rebindUp, startX + step * 0.f, rebindRowY);
        placeCentered(*_rebindDown, startX + step * 1.f, rebindRowY);
        placeCentered(*_rebindLeft, startX + step * 2.f, rebindRowY);
        placeCentered(*_rebindRight, startX + step * 3.f, rebindRowY);
        placeCentered(*_rebindShoot, startX + step * 4.f, rebindRowY);

        const float labelOffsetY = ch * 0.08f;
        auto placeRebindLabel = [&](Graphics::IText &t, float cxBtn) {
            t.setPosition(cxBtn - t.getWidth() * 0.5f, rebindRowY + labelOffsetY);
        };
        placeRebindLabel(*_rebindUpLabel, startX + step * 0.f);
        placeRebindLabel(*_rebindDownLabel, startX + step * 1.f);
        placeRebindLabel(*_rebindLeftLabel, startX + step * 2.f);
        placeRebindLabel(*_rebindRightLabel, startX + step * 3.f);
        placeRebindLabel(*_rebindShootLabel, startX + step * 4.f);

        const float bottomY = cy0 + ch * 0.80f;
        const float titleGap = ch * 0.05f;

        if (_colorBlindTitleText)
            setCenteredText(*_colorBlindTitleText, cx0 + cw * 0.20f, bottomY - titleGap, 18.f);
        if (_resolutionTitleText)
            setCenteredText(*_resolutionTitleText, cx0 + cw * 0.50f, bottomY - titleGap, 18.f);
        if (_controlsTitleText)
            setCenteredText(*_controlsTitleText, cx0 + cw * 0.80f, bottomY - titleGap, 18.f);

        placeCentered(*_colorBlindCycle, cx0 + cw * 0.20f, bottomY);
        placeCentered(*_resolutionCycle, cx0 + cw * 0.50f, bottomY);
        placeCentered(*_controlsCycle, cx0 + cw * 0.80f, bottomY);

        placeCentered(*_back, inner.cx(), cy0 + ch * 0.94f);
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
            if (std::cmp_greater_equal(_errorFrameCount, ERROR_DISPLAY_FRAMES)) {
                if (UI::UIButton *button = _rebindButtonMap[_errorState])
                    button->setLabel("...");
                _errorState = RebindState::None;
                _errorFrameCount = 0;
            }
        }

        updateButtons(frame.mouseX, frame.mouseY, _back.get(), _musicVolUp.get(), _musicVolDown.get(), _sfxVolUp.get(),
            _sfxVolDown.get(), _muteMusic.get(), _muteSFX.get(), _colorBlindCycle.get(), _resolutionCycle.get(),
            _controlsCycle.get(), _rebindUp.get(), _rebindDown.get(), _rebindLeft.get(), _rebindRight.get(),
            _rebindShoot.get());
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
                const auto action = toBindAction(_rebindState);
                _rebindButtonMap[_rebindState]->setLabel(Utils::SettingsConfig::keyToString(config.getKey(action)));
                _rebindState = RebindState::None;
                return;
            }
            auto currentKey = Key::Unknown;
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
                if (UI::UIButton *button = _rebindButtonMap[_rebindState])
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
                _resolutionChanged = true;
                break;
            }
            default: break;
        }
    }

    void SettingsMenu::handleMousePressed(const InputFrame &frame) const
    {
        pressButtons(frame.mouseX, frame.mouseY, _back.get(), _musicVolUp.get(), _musicVolDown.get(), _sfxVolUp.get(),
            _sfxVolDown.get(), _muteMusic.get(), _muteSFX.get(), _colorBlindCycle.get(), _resolutionCycle.get(),
            _controlsCycle.get(), _rebindUp.get(), _rebindDown.get(), _rebindLeft.get(), _rebindRight.get(),
            _rebindShoot.get());
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
        if (_colorBlindCycle->onMouseReleased(mx, my)) {
            _currentColorBlindMode = nextMode(_currentColorBlindMode);
            _renderer->setColorBlindMode(_currentColorBlindMode);
            config.setColorBlindMode(_currentColorBlindMode);
            _colorBlindCycle->setLabel(colorBlindLabel(_currentColorBlindMode));
            _colorBlindCycle->reset();
            return true;
        }
        if (_resolutionCycle->onMouseReleased(mx, my)) {
            _currentResolution = (_currentResolution + 1) % _resolutions.size();
            _resolutionChanged = true;
            config.setResolution(_resolutions.at(_currentResolution));
            const auto &[width, height] = _resolutions.at(_currentResolution);
            _resolutionCycle->setLabel(std::to_string(width) + "x" + std::to_string(height));
            _resolutionCycle->reset();
            return true;
        }
        return false;
    }

    bool SettingsMenu::handleControlsReleased(const float mx, const float my)
    {
        if (!_controlsCycle->onMouseReleased(mx, my))
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
        _controlsCycle->setLabel(Utils::SettingsConfig::getPresetName(next));
        updateRebindLabels();
        _controlsChanged = true;
        _controlsCycle->reset();
        return true;
    }

    bool SettingsMenu::handleAudioReleased(const float mx, const float my)
    {
        auto &config = Utils::SettingsConfig::getInstance();
        if (_musicVolUp->onMouseReleased(mx, my)) {
            _musicVolume = std::min<size_t>(100, _musicVolume + 10);
            _musicVolValueText->setString(std::to_string(_musicVolume));
            config.setMusicVolume(_musicVolume);
            applyMusicVolumeChange(_musicVolume, _musicMuted);
            _musicVolUp->reset();
            return true;
        }
        if (_musicVolDown->onMouseReleased(mx, my)) {
            _musicVolume = (_musicVolume >= 10) ? _musicVolume - 10 : 0;
            _musicVolValueText->setString(std::to_string(_musicVolume));
            config.setMusicVolume(_musicVolume);
            applyMusicVolumeChange(_musicVolume, _musicMuted);
            _musicVolDown->reset();
            return true;
        }
        if (_sfxVolUp->onMouseReleased(mx, my)) {
            _sfxVolume = std::min<size_t>(100, _sfxVolume + 10);
            _sfxVolValueText->setString(std::to_string(_sfxVolume));
            config.setSfxVolume(_sfxVolume);
            applySoundVolumeChange(_sfxVolume, _sfxMuted);
            _sfxVolUp->reset();
            return true;
        }
        if (_sfxVolDown->onMouseReleased(mx, my)) {
            _sfxVolume = (_sfxVolume >= 10) ? _sfxVolume - 10 : 0;
            _sfxVolValueText->setString(std::to_string(_sfxVolume));
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
            if (button && button->onMouseReleased(mx, my)) {
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
        if (_panelTex != Graphics::InvalidTexture)
            _renderer->draw(_panelCmd);
        _renderer->draw(*_title);
        _renderer->draw(*_musicVolValueText);
        _renderer->draw(*_sfxVolValueText);

        _musicVolUp->render();
        _musicVolDown->render();
        _sfxVolUp->render();
        _sfxVolDown->render();
        _muteMusic->render();
        _muteSFX->render();

        if (_colorBlindTitleText)
            _renderer->draw(*_colorBlindTitleText);
        if (_resolutionTitleText)
            _renderer->draw(*_resolutionTitleText);
        if (_controlsTitleText)
            _renderer->draw(*_controlsTitleText);

        _colorBlindCycle->render();
        _resolutionCycle->render();
        _controlsCycle->render();

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
        _back->render();
    }
} // namespace Engine