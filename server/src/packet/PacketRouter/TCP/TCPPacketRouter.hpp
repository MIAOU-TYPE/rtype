/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCPPacketRouter
*/

#pragma once
#include <memory>
#include "RoomManager.hpp"
#include "TCPPacketFactory.hpp"
#include "TCPTypesData.hpp"

namespace Net
{
    /**
     * @class TCPPacketRouter
     * @brief Routes incoming TCP packets to appropriate handlers
     */
    class TCPPacketRouter {
      public:
        /**
         * @brief Constructor for TCPPacketRouter
         * @param sessions The session manager
         * @param rooms The room manager
         * @param tcpServer The TCP server
         * @param packetFactory The TCP packet factory for outgoing packets
         */
        TCPPacketRouter(std::shared_ptr<Server::ISessionManager> sessions, std::shared_ptr<Engine::RoomManager> rooms,
            std::shared_ptr<Server::IServer> tcpServer, std::shared_ptr<Factory::TCPPacketFactory> packetFactory);

        /**
         * @brief Handles an incoming TCP packet
         * @param pkt The incoming packet to handle
         */
        void handle(const std::shared_ptr<IPacket> &pkt) const;

      private:
        /**
         * @brief Sends an error response to the client
         * @param addr The address of the client
         * @param req The request ID
         * @param code The error code
         * @param msg The error message
         */
        void sendError(const sockaddr_in &addr, uint32_t req, uint16_t code, std::string_view msg) const;

        /**
         * @brief Handles the HELLO packet from a client
         * @param addr The address of the client
         * @param sessionId The session ID of the client
         * @param req The request ID
         * @param r The TCP reader for the packet body
         */
        void onHello(const sockaddr_in &addr, int sessionId, uint32_t req, TCP::Reader &r) const;

        /**
         * @brief Handles the LIST_ROOMS packet from a client
         * @param addr The address of the client
         * @param req The request ID
         */
        void onListRooms(const sockaddr_in &addr, uint32_t req) const;

        /**
         * @brief Handles the CREATE_ROOM packet from a client
         * @param addr The address of the client
         * @param req The request ID
         * @param r The TCP reader for the packet body
         */
        void onCreateRoom(const sockaddr_in &addr, uint32_t req, TCP::Reader &r) const;

        /**
         * @brief Handles the JOIN_ROOM packet from a client
         * @param addr The address of the client
         * @param sessionId The session ID of the client
         * @param req The request ID
         * @param r The TCP reader for the packet body
         */
        void onJoinRoom(const sockaddr_in &addr, int sessionId, uint32_t req, TCP::Reader &r) const;

        /**
         * @brief Handles the LEAVE_ROOM packet from a client
         * @param addr The address of the client
         * @param sessionId The session ID of the client
         * @param req The request ID
         */
        void onLeaveRoom(const sockaddr_in &addr, int sessionId, uint32_t req) const;

        /**
         * @brief Handles the START_GAME packet from a client
         * @param addr The address of the client
         * @param sessionId The session ID of the client
         */
        void onStartGame(const sockaddr_in &addr, int sessionId) const;

        std::shared_ptr<Server::ISessionManager> _sessions = nullptr;        ///> Session manager
        std::shared_ptr<Engine::RoomManager> _rooms = nullptr;               ///> Room manager
        std::shared_ptr<Server::IServer> _tcp = nullptr;                     ///> TCP server
        std::shared_ptr<Factory::TCPPacketFactory> _packetFactory = nullptr; ///> TCP packet factory
    };
} // namespace Net
