/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SignalHandler
*/

#include "SignalHandler.hpp"
using namespace Signal;

#ifdef _WIN32

SignalHandler::SignalHandler()
{
    _handler = std::make_unique<WinHandler>();
}

void SignalHandler::registerCallback(SignalType type, std::function<void()> callback)
{
    _handler->registerCallback(type, callback);
}

void SignalHandler::start()
{
    _handler->start();
}

void SignalHandler::stop()
{
    _handler->stop();
}
#endif

#ifndef _WIN32
SignalHandler::SignalHandler()
{
    _handler = std::make_unique<PosixHandler>();
}

void SignalHandler::registerCallback(SignalType type, std::function<void()> callback)
{
    _handler->registerCallback(type, callback);
}

void SignalHandler::start()
{
    _handler->start();
}

void SignalHandler::stop()
{
    _handler->stop();
}
#endif
