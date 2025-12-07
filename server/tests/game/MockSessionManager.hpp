/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MockSessionManager
*/

#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include "network/SessionManager/SessionManager.hpp"

class MockSessionManager : public Net::Server::SessionManager {
public:
    sockaddr_in fakeAddr{};

    MockSessionManager() {
        fakeAddr.sin_family = AF_INET;
        fakeAddr.sin_port = htons(1234);
        inet_pton(AF_INET, "127.0.0.1", &fakeAddr.sin_addr);
    }

    const sockaddr_in *getAddress(int) const {
        return &fakeAddr;
    }
};
