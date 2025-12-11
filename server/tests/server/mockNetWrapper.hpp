/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** mockNetWrapper
*/

#pragma once

#include <gmock/gmock.h>
#include "NetWrapper.hpp"

class MockNetWrapper : public Net::NetWrapper {
  public:
    MOCK_METHOD(Net::socket_t, createSocket, (int domain, int type, int protocol), ());
    MOCK_METHOD(int, bindSocket, (Net::socket_t sock, const Net::SocketConfig &cfg), ());
    MOCK_METHOD(int, closeSocket, (Net::socket_t sock), ());
    MOCK_METHOD(int, sendTo, (Net::socket_t sock, const void *buf, size_t len, const Net::SocketConfig &cfg), ());
    MOCK_METHOD(int, recvFrom, (Net::socket_t sock, void *buf, size_t len, Net::SocketConfig &cfg), ());
    MOCK_METHOD(bool, isValidSocket, (Net::socket_t sock), (const));
};
