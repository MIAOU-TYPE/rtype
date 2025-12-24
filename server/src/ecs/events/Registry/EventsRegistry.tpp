#pragma once

namespace Ecs
{
    template <typename Event>
void EventsRegistry::subscribe(std::function<void(const Event&)> callback)
    {
        auto& vec = _subscribers[typeid(Event)];
        vec.emplace_back(
            [cb = std::move(callback)](const void* evt) {
                cb(*static_cast<const Event*>(evt));
            }
        );
    }


    template <typename Event>
 void EventsRegistry::emit(const Event& event)
    {
        _queue.push(QueuedEvent{
            typeid(Event),
            std::make_shared<Event>(event)
        });
    }

}
