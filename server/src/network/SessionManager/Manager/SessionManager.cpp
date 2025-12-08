/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager.hpp
*/

#include "SessionManager.hpp"

#include <netinet/in.h>
using namespace Net::Server;

bool AddressKey::operator==(const AddressKey &other) const noexcept
{
    return ip == other.ip && port == other.port;
}

std::size_t AddressKeyHash::operator()(const AddressKey &k) const noexcept
{
    return std::hash<uint64_t>{}((static_cast<uint64_t>(k.ip) << 16) | k.port);
}

int SessionManager::getOrCreateSession(const AddressIn &address)
{
    std::lock_guard<std::mutex> lock(_mutex);

    sockaddr_in addr = address;

    AddressKey key{addr.sin_addr.s_addr, addr.sin_port};

    auto it = _addressToId.find(key);
    if (it != _addressToId.end())
        return it->second;

    int newId = _nextId++;
    _addressToId[key] = newId;
    _idToAddress[newId] = addr;
    return newId;
}

int SessionManager::getSessionId(const AddressIn &address) const
{
    sockaddr_in addr = address;
    AddressKey key{addr.sin_addr.s_addr, addr.sin_port};

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

    sockaddr_in addr = it->second;
    AddressKey key{addr.sin_addr.s_addr, addr.sin_port};

    _idToAddress.erase(sessionId);
    _addressToId.erase(key);
}

const AddressIn *SessionManager::getAddress(int sessionId) const
{
    auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return nullptr;
    return &it->second;
}