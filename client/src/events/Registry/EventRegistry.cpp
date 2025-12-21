/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EventRegistry
*/

#include "EventRegistry.hpp"

namespace Engine
{
    EventRegistry::EventRegistry(const std::shared_ptr<EventBus> &bus) : _bus(bus)
    {
        _bus->on<KeyPressed>([this](const KeyPressed &e) {
            const auto it = _keyPressed.find(e.key);
            if (it == _keyPressed.end())
                return;

            for (auto &fn : it->second)
                fn();
        });

        _bus->on<KeyReleased>([this](const KeyReleased &e) {
            const auto it = _keyReleased.find(e.key);
            if (it == _keyReleased.end())
                return;

            for (auto &fn : it->second)
                fn();
        });
    }

    void EventRegistry::onKeyPressed(const Key key, std::function<void()> fn)
    {
        _keyPressed[key].push_back(std::move(fn));
    }

    void EventRegistry::onKeyReleased(const Key key, std::function<void()> fn)
    {
        _keyReleased[key].push_back(std::move(fn));
    }
} // namespace Engine
