/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EventBus
*/

#pragma once

namespace Engine
{
    template <typename T>
    void EventBus::on(Handler<T> handler)
    {
        _handlers[typeid(T)].emplace_back([h = std::move(handler)](const void *e) {
            h(*static_cast<const T *>(e));
        });
    }

    template <typename T>
    void EventBus::emit(T event)
    {
        std::scoped_lock lock(_queueMutex);

        _queue.push(QueuedEvent{typeid(T), [this, e = std::move(event)]() {
                                    const auto it = _handlers.find(typeid(T));
                                    if (it == _handlers.end())
                                        return;

                                    for (auto &fn : it->second)
                                        fn(&e);
                                }});
    }

    inline void EventBus::dispatch()
    {
        std::queue<QueuedEvent> local;

        {
            std::scoped_lock lock(_queueMutex);
            std::swap(local, _queue);
        }

        while (!local.empty()) {
            local.front().invoke();
            local.pop();
        }
    }
} // namespace Engine
