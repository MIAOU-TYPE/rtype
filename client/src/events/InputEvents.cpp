/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** InputEvents
*/

#include "InputEvents.hpp"
#include <algorithm>

using namespace Events;

void InputEventManager::registerHandler(InputAction action, const std::shared_ptr<InputEventHandler>& handler)
{
    if (!handler) {
        return;
    }

    _handlers[action].push_back(handler);
}

void InputEventManager::unregisterHandler(InputAction action, std::shared_ptr<InputEventHandler> handler)
{
    if (!handler) {
        return;
    }

    auto &handlers = _handlers[action];
    handlers.erase(std::ranges::remove_if(handlers,
                       [&handler](const std::shared_ptr<InputEventHandler> &h) {
                           return h == handler;
                       }),
        handlers.end());
}

void InputEventManager::dispatchEvent(const InputEvent &event)
{
    if (event.state == InputState::Pressed) {
        _heldActions[event.action] = true;
        _heldTimes[event.action] = 0.0f;
    } else if (event.state == InputState::Released) {
        _heldActions[event.action] = false;
        _heldTimes[event.action] = 0.0f;
    }

    auto it = _handlers.find(event.action);
    if (it != _handlers.end()) {
        auto &handlers = it->second;
        for (auto &handler : handlers) {
            if (handler) {
                handler->onInputEvent(event);
            }
        }
    }
}

void InputEventManager::updateHeldActions(float deltaTime)
{
    for (auto &[action, isHeld] : _heldActions) {
        if (isHeld) {
            _heldTimes[action] += deltaTime;

            InputEvent heldEvent;
            heldEvent.action = action;
            heldEvent.state = InputState::Held;
            heldEvent.deltaTime = _heldTimes[action];

            auto it = _handlers.find(action);
            if (it != _handlers.end()) {
                auto &handlers = it->second;
                for (auto &handler : handlers) {
                    if (handler) {
                        handler->onInputEvent(heldEvent);
                    }
                }
            }
        }
    }
}