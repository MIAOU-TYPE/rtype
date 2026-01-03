/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager
*/

#include "SessionManager.hpp"

using namespace Net::Server;

int SessionManager::getOrCreateSession(const sockaddr_in &address)
{
    const AddressKey key{address.sin_addr.s_addr, address.sin_port};

    {
        std::shared_lock rLock(_mutex);
        if (const auto it = _tcpAddressToId.find(key); it != _tcpAddressToId.end())
            return it->second;
    }

    std::unique_lock wLock(_mutex);

    if (const auto it = _tcpAddressToId.find(key); it != _tcpAddressToId.end())
        return it->second;

    const int newId = _nextId++;
    _tcpAddressToId[key] = newId;
    _idToTcpAddress[newId] = address;
    return newId;
}

int SessionManager::getSessionId(const sockaddr_in &address) const
{
    const AddressKey key{address.sin_addr.s_addr, address.sin_port};

    std::shared_lock lock(_mutex);
    if (const auto it = _tcpAddressToId.find(key); it != _tcpAddressToId.end())
        return it->second;

    return -1;
}

void SessionManager::removeSession(const int sessionId)
{
    std::unique_lock lock(_mutex);

    if (const auto itTcp = _idToTcpAddress.find(sessionId); itTcp != _idToTcpAddress.end()) {
        const AddressKey tcpKey{itTcp->second.sin_addr.s_addr, itTcp->second.sin_port};
        _idToTcpAddress.erase(itTcp);
        _tcpAddressToId.erase(tcpKey);
    } else {
        return;
    }

    if (const auto itUdp = _idToUdpAddress.find(sessionId); itUdp != _idToUdpAddress.end()) {
        const AddressKey udpKey{itUdp->second.sin_addr.s_addr, itUdp->second.sin_port};
        _idToUdpAddress.erase(itUdp);
        _udpAddressToId.erase(udpKey);
    }

    _udpTokenById.erase(sessionId);
}

const sockaddr_in *SessionManager::getAddress(const int sessionId) const
{
    std::shared_lock lock(_mutex);

    const auto it = _idToTcpAddress.find(sessionId);
    if (it == _idToTcpAddress.end())
        return nullptr;
    return &it->second;
}

std::vector<std::pair<int, sockaddr_in>> SessionManager::getAllSessions() const
{
    std::shared_lock lock(_mutex);
    std::vector<std::pair<int, sockaddr_in>> list;
    list.reserve(_idToTcpAddress.size());

    for (const auto &[id, addr] : _idToTcpAddress)
        list.emplace_back(id, addr);
    return list;
}

void SessionManager::forEachSession(const std::function<void(int, const sockaddr_in &)> &func) const
{
    std::vector<std::pair<int, sockaddr_in>> snapshot;

    {
        std::shared_lock lock(_mutex);
        snapshot.reserve(_idToTcpAddress.size());

        for (const auto &[id, addr] : _idToTcpAddress)
            snapshot.emplace_back(id, addr);
    }

    for (auto &[id, addr] : snapshot)
        func(id, addr);
}

void SessionManager::setUdpToken(const int sessionId, const uint64_t token)
{
    std::unique_lock lock(_mutex);
    _udpTokenById[sessionId] = token;
}

uint64_t SessionManager::getUdpToken(const int sessionId) const
{
    std::shared_lock lock(_mutex);
    if (const auto it = _udpTokenById.find(sessionId); it != _udpTokenById.end())
        return it->second;
    return 0;
}

bool SessionManager::bindUdp(const int sessionId, const sockaddr_in &udpAddr)
{
    std::unique_lock lock(_mutex);

    if (!_idToTcpAddress.contains(sessionId))
        return false;

    if (const auto itOld = _idToUdpAddress.find(sessionId); itOld != _idToUdpAddress.end()) {
        const AddressKey oldKey{itOld->second.sin_addr.s_addr, itOld->second.sin_port};
        _udpAddressToId.erase(oldKey);
        _idToUdpAddress.erase(itOld);
    }

    const AddressKey newKey{udpAddr.sin_addr.s_addr, udpAddr.sin_port};
    _idToUdpAddress[sessionId] = udpAddr;
    _udpAddressToId[newKey] = sessionId;

    return true;
}

const sockaddr_in *SessionManager::getUdpAddress(const int sessionId) const
{
    std::shared_lock lock(_mutex);

    const auto it = _idToUdpAddress.find(sessionId);
    if (it == _idToUdpAddress.end())
        return nullptr;
    return &it->second;
}

int SessionManager::getSessionIdFromUdp(const sockaddr_in &udpAddr) const
{
    const AddressKey key{udpAddr.sin_addr.s_addr, udpAddr.sin_port};

    std::shared_lock lock(_mutex);
    if (const auto it = _udpAddressToId.find(key); it != _udpAddressToId.end())
        return it->second;

    return -1;
}
