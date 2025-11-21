/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** WinHandler
*/

#ifdef _WIN32

    #include "WinHandler.hpp"

    #include <csignal>
    #include <iostream>

using namespace Signal;

WinHandler *WinHandler::instance = nullptr;

SignalType fromNativeSignal(int signum)
{
    switch (signum) {
        case SIGINT: return SignalType::Interrupt;
        case SIGTERM: return SignalType::Terminate;
        default: return SignalType::Terminate;
    }
}

WinHandler::WinHandler() : running(false)
{
    instance = this;
}

WinHandler::~WinHandler()
{
    stop();
    if (instance == this)
        instance = nullptr;
}

void WinHandler::start() noexcept
{
    running = true;

    std::signal(SIGINT, &WinHandler::handleSignal);
    std::signal(SIGTERM, &WinHandler::handleSignal);
}

void WinHandler::stop() noexcept
{
    if (!running.load())
        return;

    running = false;

    std::signal(SIGINT, SIG_DFL);
    std::signal(SIGTERM, SIG_DFL);
}

void WinHandler::registerCallback(SignalType type, std::function<void()> callback) noexcept
{
    callbacks[type] = std::move(callback);
}

void WinHandler::handleSignal(int signum)
{
    if (!instance || !instance->running.load())
        return;

    SignalType type = fromNativeSignal(signum);

    auto it = instance->callbacks.find(type);
    if (it == instance->callbacks.end())
        return;

    auto &cb = it->second;
    if (!cb)
        return;

    try {
        cb();
    } catch (const std::exception &e) {
        std::cerr << "[WinHandler] Exception in signal callback: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[WinHandler] Unknown exception in signal callback" << std::endl;
    }
}

#endif // _WIN32
