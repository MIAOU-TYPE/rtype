/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** packetReceived
*/

#pragma once
#include <vector>
#include <netinet/in.h>

struct PacketReceived {
    std::vector<unsigned char> data;
    size_t size;
    sockaddr_in addr;
};
