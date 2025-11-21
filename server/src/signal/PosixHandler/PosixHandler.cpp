/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PosixHandler
*/

#ifndef _WIN32

    #include "PosixHandler.hpp"
    #include <iostream>

using namespace Signal;

PosixHandler *PosixHandler::instance = nullptr;

PosixHandler::PosixHandler() : running(false)
{
    instance = this;
}

PosixHandler::~PosixHandler()
{
    stop();
}

void PosixHandler::start() noexcept
{
    try {
        running = true;

        struct sigaction action{};
        action.sa_handler = PosixHandler::handleSignal;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;

        sigaction(SIGINT, &action, nullptr);
        sigaction(SIGTERM, &action, nullptr);
        sigaction(SIGHUP, &action, nullptr);
    } catch (const std::exception &e) {
        std::cerr << "{PosixHandler} Failed to start signal handler" << std::endl;
        running = false;
    }
}

void PosixHandler::stop() noexcept
{
    if (!running.load())
        return;
    running = false;

    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGHUP, SIG_DFL);
}

void PosixHandler::registerCallback(SignalType type, std::function<void()> callback) noexcept
{
    std::lock_guard<std::mutex> lock(mutex);

    switch (type) {
        case SignalType::Interrupt: _handlers[SignalType::Interrupt].push_back(callback); break;
        case SignalType::Terminate: _handlers[SignalType::Terminate].push_back(callback); break;
        case SignalType::Hangup: _handlers[SignalType::Hangup].push_back(callback); break;
    }
}

void PosixHandler::handleSignal(int sig)
{
    if (!instance || !instance->running.load())
        return;

    switch (sig) {
        case SIGINT: instance->dispatch(SignalType::Interrupt); break;
        case SIGTERM: instance->dispatch(SignalType::Terminate); break;
        case SIGHUP: instance->dispatch(SignalType::Hangup); break;
    }
}

void PosixHandler::dispatch(SignalType type)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto &handlers = _handlers[type];
    for (auto &fn : handlers)
        fn();
}

#endif
