/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace Engine
{
    SettingsState::SettingsState(StateManager &manager, std::shared_ptr<Graphics::IGraphics> graphics,
        std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<MusicRegistry> musicRegistry,
        std::shared_ptr<SoundRegistry> soundRegistry)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _manager(manager)
    {
    }

    void SettingsState::onEnter(StateManager &manager)
    {
        (void) manager;
        _menu = std::make_unique<SettingsMenu>(_renderer, _musicRegistry, _soundRegistry);
        _menu->onEnter();
    }

    void SettingsState::update(const InputFrame &frame)
    {
        if (_pendingResize) {
            _menu->layout();
            _pendingResize = false;
        }
        _menu->update(frame);
        if (_menu->resolutionChanged()) {
            _graphics->setResolution(_menu->currentResolution());
            _pendingResize = true;
        }
        if (_menu->wantsBack())
            _manager.queueState(std::make_unique<MenuState>(_graphics, _renderer, _musicRegistry, _soundRegistry));
    }

    void SettingsState::render()
    {
        _menu->render();
    }
} // namespace Engine
