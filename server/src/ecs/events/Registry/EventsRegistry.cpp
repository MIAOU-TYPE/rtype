/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EventsRegistry
*/

#include "EventsRegistry.hpp"

namespace Ecs
{
    void EventsRegistry::process()
    {
        while (!_queue.empty()) {
            const auto qe = std::move(_queue.front());
            _queue.pop();

            const auto it = _subscribers.find(qe.type);
            if (it == _subscribers.end())
                continue;

            for (const auto &fn : it->second)
                fn(qe.data.get());
        }
    }

} // namespace Ecs