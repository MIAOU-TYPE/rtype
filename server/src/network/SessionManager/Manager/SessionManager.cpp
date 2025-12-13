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
    const AddressKey key{address.sin_addr.s_addr, address.sin_port};

    {
        std::shared_lock rLock(_mutex);
        if (const auto it = _addressToId.find(key); it != _addressToId.end())
            return it->second;
    }

    std::unique_lock wLock(_mutex);

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

    std::shared_lock lock(_mutex);
    if (const auto it = _addressToId.find(key); it != _addressToId.end())
        return it->second;

    return -1;
}

void SessionManager::removeSession(const int sessionId)
{
    std::unique_lock lock(_mutex);

    const auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return;

    const AddressKey key{it->second.sin_addr.s_addr, it->second.sin_port};

    _idToAddress.erase(sessionId);
    _addressToId.erase(key);
}

const sockaddr_in *SessionManager::getAddress(int sessionId) const
{
    std::shared_lock lock(_mutex);

    const auto it = _idToAddress.find(sessionId);
    if (it == _idToAddress.end())
        return nullptr;
    return &it->second;
}

std::vector<std::pair<int, sockaddr_in>> SessionManager::getAllSessions() const
{
    std::shared_lock lock(_mutex);
    std::vector<std::pair<int, sockaddr_in>> list;
    list.reserve(_idToAddress.size());

    for (const auto &[fst, snd] : _idToAddress)
        list.emplace_back(fst, snd);
    return list;
}

void SessionManager::forEachSession(const std::function<void(int, const sockaddr_in &)> &func) const
{
    std::vector<std::pair<int, sockaddr_in>> snapshot;

    {
        std::shared_lock lock(_mutex);
        snapshot.reserve(_idToAddress.size());

        for (auto &[id, addr] : _idToAddress)
            snapshot.emplace_back(id, addr);
    }

    for (auto &[id, addr] : snapshot)
        func(id, addr);
}