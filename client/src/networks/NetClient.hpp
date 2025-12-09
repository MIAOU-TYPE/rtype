/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** NetClient
*/

#pragma once

#include "InputData.hpp"
#include "NetWrapper.hpp"
#include "TypesData.hpp"
#include "UDPPacket.hpp"

namespace Game
{
    class NetClient {
      public:
        NetClient();
        ~NetClient();

        void sendConnectPacket();
        void sendInputPacket(float dx, float dy, bool shooting);
        void receivePackets();
        void handlePacket(const Net::UDPPacket &packet);
        void sendDisconnectPacket();
        void sendPingPacket();
        void close();

      private:
        std::unique_ptr<Net::NetWrapper> _netWrapper = nullptr; ///> Network wrapper
        socketHandle _socket = kInvalidSocket;                  ///> UDP socket
        sockaddr_in _serverAddr = {};                           ///> Server address
        bool _isConnected = false;                              ///> Connection status
        uint32_t _playerEntityId = 0;                           ///> Player entity ID
    };

} // namespace Game