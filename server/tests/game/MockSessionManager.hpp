/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MockSessionManager
*/

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include "ISessionManager.hpp"

class MockSessionManager : public Net::Server::ISessionManager {
  public:
    Net::Server::AddressIn fakeAddr{};

    MockSessionManager()
    {
        fakeAddr.port = htons(1234);
        inet_pton(AF_INET, "127.0.0.1", &fakeAddr.address);
    }

    const Net::Server::AddressIn *getAddress(int id) const override
    {
        return &fakeAddr;
    }

    int getOrCreateSession(const Net::Server::AddressIn &addr) override
    {
        return 1;
    };

    int getSessionId(const Net::Server::AddressIn &addr) const override
    {
        return 1;
    };

    void removeSession(int sessionId) override
    {
        return;
    };
};
