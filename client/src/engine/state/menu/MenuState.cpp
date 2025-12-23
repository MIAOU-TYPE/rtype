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
        std::shared_ptr<InputState> input)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _input(std::move(input))
    {
    }

    void MenuState::onEnter(StateManager &manager)
    {
        _manager = &manager;
        _menu = std::make_unique<Menu>(_renderer);
    }

    void MenuState::onExit()
    {
        _menu.reset();
    }

    void MenuState::update(const float dt)
    {
        _menu->update(dt, _input->mouseX, _input->mouseY);

        if (_input->mouseLeftPressed)
            _menu->onMousePressed(_input->mouseX, _input->mouseY);

        if (_input->mouseLeftReleased)
            _menu->onMouseReleased(_input->mouseX, _input->mouseY);

        if (_menu->wantsSettings()) {
            _manager->changeState(std::make_unique<SettingsState>(_graphics, _renderer, _input));
        }
    }

    void MenuState::render()
    {
        _menu->render();
    }

    bool MenuState::onMousePressed(const float x, const float y)
    {
        if (_menu)
            return _menu->onMousePressed(x, y);
        return false;
    }

    bool MenuState::onMouseReleased(const float x, const float y)
    {
        if (_menu)
            return _menu->onMouseReleased(x, y);
        return false;
    }
} // namespace Engine
