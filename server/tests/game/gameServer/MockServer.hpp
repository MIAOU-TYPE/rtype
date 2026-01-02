/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MockServer
*/

#pragma once
#include "IServer.hpp"

class MockServer : public Net::Server::IServer {
  public:
    bool sent = false;

    void configure(const std::string &, int32_t) override
    {
    }

    void setNonBlocking(bool) noexcept override
    {
    }

    bool isStoredIpCorrect() const noexcept override
    {
        return true;
    }

    bool isStoredPortCorrect() const noexcept override
    {
        return true;
    }

    bool sendPacket(const Net::IPacket &) noexcept override
    {
        sent = true;
        return true;
    }

    void start() override
    {
    }

    void stop() noexcept override
    {
    }

    bool isRunning() const noexcept override
    {
        return false;
    }

    void setRunning(bool) noexcept override
    {
    }

    void readPackets() noexcept override
    {
    }

    bool popPacket(std::shared_ptr<Net::IPacket> &) noexcept override
    {
        return false;
    }
};
