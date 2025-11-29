/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SignalHandler
*/

#include "SignalHandler.hpp"
using namespace Signal;

void SignalHandler::registerCallback(SignalType type, std::function<void()> callback) noexcept
{
    _handler->registerCallback(type, callback);
}

void SignalHandler::start() noexcept
{
    _handler->start();
}

void SignalHandler::stop() noexcept
{
    _handler->stop();
    _handler = nullptr;
}

SignalHandler::~SignalHandler()
{
    stop();
}

#ifdef _WIN32

SignalHandler::SignalHandler()
{
    _handler = std::make_unique<WinHandler>();
}

#endif

#ifndef _WIN32
SignalHandler::SignalHandler()
{
    _handler = std::make_unique<PosixHandler>();
}
#endif
