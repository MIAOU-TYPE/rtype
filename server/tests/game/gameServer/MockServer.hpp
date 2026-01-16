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

    [[nodiscard]] bool isStoredIpCorrect() const noexcept override
    {
        return true;
    }

    [[nodiscard]] bool isStoredPortCorrect() const noexcept override
    {
        return true;
    }

    [[nodiscard]] bool sendPacket(const Net::IPacket &) noexcept override
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

    [[nodiscard]] bool isRunning() const noexcept override
    {
        return false;
    }

    void setRunning(bool) noexcept override
    {
    }

    [[nodiscard]] bool readPackets() noexcept override
    {
    }

    [[nodiscard]] bool popPacket(std::shared_ptr<Net::IPacket> &) noexcept override
    {
        return false;
    }

    [[nodiscard]] int32_t getPort() const noexcept override
    {
        return 0;
    }
};
