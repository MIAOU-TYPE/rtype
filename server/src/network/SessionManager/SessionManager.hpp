/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager
*/

#pragma once

#include <unordered_map>
#include <cstdint>
#include <mutex>
#include <netinet/in.h>

struct AddressKey {
    uint32_t ip;
    uint16_t port;

    bool operator==(const AddressKey& other) const {
        return ip == other.ip && port == other.port;
    }
};

struct AddressKeyHash {
    std::size_t operator()(const AddressKey& k) const noexcept {
        return std::hash<uint64_t>{}(
            (static_cast<uint64_t>(k.ip) << 16) | k.port
        );
    }
};

class SessionManager {
public:
    int getOrCreateSession(const sockaddr_in& addr);


    int getSessionId(const sockaddr_in& addr) const;

    void removeSession(int sessionId);

    const sockaddr_in* getAddress(int sessionId) const;

private:
    mutable std::mutex _mutex = {};

    std::unordered_map<AddressKey, int, AddressKeyHash> _addressToId = {};
    std::unordered_map<int, sockaddr_in> _idToAddress = {};

    int _nextId = 1;
};
