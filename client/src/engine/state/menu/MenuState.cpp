/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#include "MenuState.hpp"

namespace Engine
{
    MenuState::MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
        std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry))
    {
    }

    void MenuState::onEnter(StateManager &manager)
    {
        try {
            _manager = &manager;
            _menu = std::make_unique<Menu>(_renderer);
            _menu->onEnter();
        } catch (const std::exception &e) {
            throw MenuError(std::string("{MenuState::onEnter} ") + e.what());
        }
    }

    void MenuState::update(const InputFrame &frame)
    {
        _menu->update(frame);

        if (_menu->wantsSettings()) {
            _manager->queueState(
                std::make_unique<SettingsState>(*_manager, _graphics, _renderer, _musicRegistry, _soundRegistry));
            return;
        }
        if (_menu->wantsToQuit())
            _manager->queueQuit();
    }

    void MenuState::render()
    {
        _menu->render();
    }
} // namespace Engine
