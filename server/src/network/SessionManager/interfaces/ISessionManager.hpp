/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ISessionManager
*/

#pragma once
#include <string>

#ifndef _WIN32
    #include <arpa/inet.h>
    #include <netinet/in.h>
#else
    #include <winsock2.h>
#endif

namespace Net::Server
{
    struct AddressIn {
        std::string address;
        uint16_t port;

        AddressIn(const std::string &addr = "", uint16_t port = 0) : address(addr), port(port)
        {
        }

        AddressIn(const sockaddr_in &addr)
        {
            char buf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
            address = buf;
            port = ntohs(addr.sin_port);
        }

        explicit operator sockaddr_in() const
        {
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
            return addr;
        }

        sockaddr_in toSockaddr() const {
            return static_cast<sockaddr_in>(*this);
        }

    };

    class ISessionManager {
      public:
        virtual ~ISessionManager() = default;

        virtual int getOrCreateSession(const AddressIn &addr) = 0;

        virtual int getSessionId(const AddressIn &addr) const = 0;

        virtual void removeSession(int sessionId) = 0;

        virtual const AddressIn *getAddress(int sessionId) const = 0;
    };
} // namespace Net::Server