#pragma once

namespace Ecs
{
    template <typename Event>
        void Events::subscribe(std::function<void(const Event &)> callback)
    {
        auto &vec = _subscribers[typeid(Event)];
        vec.emplace_back([cb = std::move(callback)](const void *evt) {
            cb(*static_cast<const Event *>(evt));
        });
    }

    template <typename Event>
            void Events::emit(const Event &event) const
    {
        const auto it = _subscribers.find(typeid(Event));
        if (it == _subscribers.end())
            return;

        for (const auto &fn : it->second)
            fn(&event);
    }
}