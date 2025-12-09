/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MockServer
*/

#pragma once
#include "server/interfaces/IServer.hpp"

class MockServer : public Net::Server::IServer {
public:
    bool sent = false;

    void configure(const std::string&, uint16_t) override {}
    void setNonBlocking(bool) noexcept override {}
    bool isStoredIpCorrect() const noexcept override { return true; }
    bool isStoredPortCorrect() const noexcept override { return true; }

    bool sendPacket(const Net::IPacket &) override {
        sent = true;
        return true;
    }

    void start() override {}
    void stop() override {}
    bool isRunning() const noexcept override { return false; }
    void setRunning(bool) noexcept override {}
    void readPackets() override {}

    bool popPacket(std::shared_ptr<Net::IPacket>&) override {
        return false;
    }
};
