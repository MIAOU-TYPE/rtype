/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MockNetWrapper for client tests
*/

#pragma once

#include <gmock/gmock.h>
#include "NetWrapper.hpp"

namespace Game
{
    /**
     * @class MockNetWrapper
     * @brief Mock implementation of NetWrapper for testing purposes
     */
    class MockNetWrapper : public Net::NetWrapper {
      public:
        MockNetWrapper() : Net::NetWrapper("NetPluginLib")
        {
        }

        MOCK_METHOD(socketHandle, socket, (int domain, int type, int protocol), (override));
        MOCK_METHOD(int, closeSocket, (socketHandle sock), (override));
        MOCK_METHOD(ssize_t, sendTo,
            (socketHandle sock, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                socklen_t addrlen),
            (override));
        MOCK_METHOD(ssize_t, recvFrom,
            (socketHandle sock, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen),
            (override));
    };

} // namespace Game
