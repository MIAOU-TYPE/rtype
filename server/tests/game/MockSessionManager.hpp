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
        fakeAddr.sin_family = AF_INET;
        fakeAddr.sin_port = htons(1234);
        inet_pton(AF_INET, "127.0.0.1", &fakeAddr.sin_addr);
    }

    const adressIn *getAddress(int) const
    {
        return &fakeAddr;
    }
};
