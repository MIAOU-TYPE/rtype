/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager.hpp
*/

#include "SessionManager.hpp"

int SessionManager::getOrCreateSession(const sockaddr_in& addr)
{
    std::lock_guard<std::mutex> lock(_mutex);

    AddressKey key{ addr.sin_addr.s_addr, addr.sin_port };

    auto it = _addressToId.find(key);
    if (it != _addressToId.end())
        return it->second;

    int newId = _nextId++;
    _addressToId[key] = newId;
    _idToAddress[newId] = addr;
    return newId;
}

int SessionManager::getSessionId(const sockaddr_in& addr) const
{
    AddressKey key{ addr.sin_addr.s_addr, addr.sin_port };

    auto it = _addressToId.find(key);
    if (it != _addressToId.end())
        return it->second;

    return -1;
}


void SessionManager::removeSession(int sessionId)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return;

    AddressKey key{ it->second.sin_addr.s_addr, it->second.sin_port };

    _idToAddress.erase(sessionId);
    _addressToId.erase(key);
}

const sockaddr_in *SessionManager::getAddress(int sessionId) const
{
    auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return nullptr;
    return &it->second;
}