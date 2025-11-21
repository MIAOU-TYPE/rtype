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
    try {
        stop();
    } catch (...) {
        std::cerr << "{WinHandler::~WinHandler} Exception occurred during destruction" << std::endl;
    }
    instance = nullptr;
}

void WinHandler::start() noexcept
{
    try {
        if (running.load())
            return;
        running = true;

        std::signal(SIGINT, &WinHandler::handleSignal);
        std::signal(SIGTERM, &WinHandler::handleSignal);
    } catch (...) {
        stop();
        std::cerr << "{WinHandler::start} Exception occurred while starting signal handler" << std::endl;
    }
}

void WinHandler::stop() noexcept
{
    try {
        running = false;

        std::signal(SIGINT, SIG_DFL);
        std::signal(SIGTERM, SIG_DFL);
    } catch (...) {
        std::cerr << "{WinHandler::stop} Exception occurred while stopping signal handler" << std::endl;
    }
}

void WinHandler::registerCallback(SignalType type, std::function<void()> callback) noexcept
{
    try {
        callbacks[type] = std::move(callback);
    } catch (...) {
        std::cerr << "{WinHandler::registerCallback} Exception occurred while registering callback" << std::endl;
    }
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
    } catch (...) {
        std::cerr << "{WinHandler::handleSignal} Unknown exception in signal callback" << std::endl;
    }
}

#endif // _WIN32
