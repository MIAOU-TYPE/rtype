/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#include "MenuState.hpp"

namespace Engine
{
    MenuState::MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer))
    {
    }

    void MenuState::onEnter(StateManager &manager)
    {
        _manager = &manager;
        _menu = std::make_unique<Menu>(_renderer);
        _menu->onEnter();
    }

    void MenuState::update(const InputFrame &frame)
    {
        _menu->update(frame);

        if (_menu->wantsSettings()) {
            _manager->queueState(std::make_unique<SettingsState>(*_manager, _graphics, _renderer));
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
