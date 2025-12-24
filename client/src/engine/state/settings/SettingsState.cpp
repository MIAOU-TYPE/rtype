/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace Engine
{
    SettingsState::SettingsState(std::shared_ptr<Graphics::IGraphics> graphics,
        std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<InputState> input)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _input(std::move(input))
    {
    }

    void SettingsState::onEnter(StateManager &manager)
    {
        _manager = &manager;
        _menu = std::make_unique<SettingsMenu>(_renderer);
        _menu->update(_input->mouseX, _input->mouseY);
    }

    void SettingsState::onExit()
    {
        _menu.reset();
    }

    void SettingsState::update()
    {
        _menu->update(_input->mouseX, _input->mouseY);
        if (_input->mouseLeftPressed)
            _menu->onMousePressed(_input->mouseX, _input->mouseY);
        if (_input->mouseLeftReleased)
            _menu->onMouseReleased(_input->mouseX, _input->mouseY);
        if (_menu->resolutionChanged())
            _graphics->setResolution(_menu->currentResolution());
        if (_input->isPressed(Key::B) || _menu->wantsBack())
            _manager->changeState(std::make_unique<MenuState>(_graphics, _renderer, _input));
    }

    void SettingsState::render()
    {
        _menu->render();
    }

    bool SettingsState::onMousePressed(const float x, const float y)
    {
        if (_menu)
            return _menu->onMousePressed(x, y);
        return false;
    }

    bool SettingsState::onMouseReleased(const float x, const float y)
    {
        if (_menu)
            return _menu->onMouseReleased(x, y);
        return false;
    }
} // namespace Engine
