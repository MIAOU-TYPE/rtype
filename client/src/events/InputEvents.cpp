/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** InputEvents
*/

#include "InputEvents.hpp"
#include <algorithm>

using namespace Events;

void InputEventManager::registerHandler(InputAction action, std::shared_ptr<InputEventHandler> handler)
{
    if (!handler) {
        return;
    }

    _handlers[action].push_back(std::weak_ptr<InputEventHandler>(handler));
}

void InputEventManager::unregisterHandler(InputAction action, std::shared_ptr<InputEventHandler> handler)
{
    if (!handler) {
        return;
    }

    auto &handlers = _handlers[action];
    handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
                       [&handler](const std::weak_ptr<InputEventHandler> &h) {
                           return h.expired() || h.lock() == handler;
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
        for (auto handlerIt = handlers.begin(); handlerIt != handlers.end();) {
            if (auto handler = handlerIt->lock()) {
                handler->onInputEvent(event);
                ++handlerIt;
            } else {
                handlerIt = handlers.erase(handlerIt);
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
                for (auto handlerIt = handlers.begin(); handlerIt != handlers.end();) {
                    if (auto handler = handlerIt->lock()) {
                        handler->onInputEvent(heldEvent);
                        ++handlerIt;
                    } else {
                        handlerIt = handlers.erase(handlerIt);
                    }
                }
            }
        }
    }
}