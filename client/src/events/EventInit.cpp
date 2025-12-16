/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** EventInit
*/

#include "EventInit.hpp"

using namespace Events;

EventInit::EventInit(std::shared_ptr<Graphics::IRenderer> renderer) : _renderer(renderer)
{
    if (!_renderer) {
        throw EventInitError("Renderer is null");
    }
}

void EventInit::run()
{
    if (!_renderer) {
        throw EventInitError("Renderer not properly initialized");
    }

    try {
        std::shared_ptr<Graphics::IEvent> event;

        while (_renderer->pollEvent(event)) {
            if (!event) {
                continue;
            }

            if (_renderer->isWindowCloseEvent(*event)) {
                _renderer->close();
                break;
            }
        }
    } catch (const std::exception &e) {
        throw EventInitError("Unexpected error during event processing: " + std::string(e.what()));
    }
}

bool EventInit::isWindowOpen() const
{
    return _renderer && _renderer->isOpen();
}
