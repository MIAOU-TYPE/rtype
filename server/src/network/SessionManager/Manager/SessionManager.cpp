/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager.hpp
*/

#include "SessionManager.hpp"

using namespace Net::Server;

bool AddressKey::operator==(const AddressKey &other) const noexcept
{
    return ip == other.ip && port == other.port;
}

std::size_t AddressKeyHash::operator()(const AddressKey &k) const noexcept
{
    return std::hash<uint64_t>{}((static_cast<uint64_t>(k.ip) << 16) | k.port);
}

int SessionManager::getOrCreateSession(const sockaddr_in &address)
{
    std::scoped_lock lock(_mutex);

    const AddressKey key{address.sin_addr.s_addr, address.sin_port};

    if (const auto it = _addressToId.find(key); it != _addressToId.end())
        return it->second;

    const int newId = _nextId++;
    _addressToId[key] = newId;
    _idToAddress[newId] = address;
    return newId;
}

int SessionManager::getSessionId(const sockaddr_in &address) const
{
    const AddressKey key{address.sin_addr.s_addr, address.sin_port};

    if (const auto it = _addressToId.find(key); it != _addressToId.end())
        return it->second;

    return -1;
}

void SessionManager::removeSession(const int sessionId)
{
    std::scoped_lock lock(_mutex);

    const auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return;

    const AddressKey key{it->second.sin_addr.s_addr, it->second.sin_port};

    _idToAddress.erase(sessionId);
    _addressToId.erase(key);
}

const sockaddr_in *SessionManager::getAddress(int sessionId) const
{
    const auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return nullptr;
    return &it->second;
}

std::vector<std::pair<int, sockaddr_in>> SessionManager::getAllSessions() const
{
    std::scoped_lock lock(_mutex);
    std::vector<std::pair<int, sockaddr_in>> list;
    list.reserve(_idToAddress.size());

    for (const auto &[fst, snd] : _idToAddress)
        list.emplace_back(fst, snd);
    return list;
}
