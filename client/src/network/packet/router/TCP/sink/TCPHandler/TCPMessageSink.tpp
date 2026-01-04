/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** TCPMessageSink
*/

#pragma once

namespace Network
{
    template <class Cbs, class... Args>
    void TCPMessageSink::emit(Cbs &cbs, Args &&...args)
    {
        for (auto &cb : cbs)
            cb(std::forward<Args>(args)...);
    }
}