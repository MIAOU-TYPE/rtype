/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ISessionManager
*/

#pragma once
#include <string>
#include <vector>

#ifndef _WIN32
    #include <arpa/inet.h>
    #include <netinet/in.h>
#else
    #include <winsock2.h>
#endif

#include <functional>
#include "UserStorage.hpp"

/**
 * @namespace Net::Server
 * @brief Namespace for server-side networking components.
 */
namespace Net::Server
{
    /**
     * @class ISessionManager
     * @brief Interface for managing network sessions.
     */
    class ISessionManager {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~ISessionManager() = default;

        /**
         * @brief Get or create a session for the given address.
         * @param addr The address of the client.
         * @return The session ID.
         */
        [[nodiscard]] virtual int getOrCreateSession(const sockaddr_in &addr) = 0;

        /**
         * @brief Get the session ID for the given address.
         * @param addr The address of the client.
         * @return The session ID, or -1 if not found.
         */
        [[nodiscard]] virtual int getSessionId(const sockaddr_in &addr) const = 0;

        /**
         * @brief Remove a session by its ID.
         * @param sessionId The ID of the session to remove.
         */
        virtual void removeSession(int sessionId) = 0;

        /**
         * @brief Get the address associated with a session ID.
         * @param sessionId The ID of the session.
         * @return Pointer to the sockaddr_in structure, or nullptr if not found.
         */
        [[nodiscard]] virtual const sockaddr_in *getAddress(int sessionId) const = 0;

        /**
         * @brief Get all active sessions.
         * @return A vector of pairs containing session IDs and their corresponding addresses.
         */
        [[nodiscard]] virtual std::vector<std::pair<int, sockaddr_in>> getAllSessions() const = 0;

        /**
         * @brief Apply a function to each session.
         * @param func The function to apply, taking session ID and address as parameters.
         */
        virtual void forEachSession(const std::function<void(int, const sockaddr_in &)> &func) const = 0;

        /**
         * @brief Set UDP token for a session.
         * @param sessionId The ID of the session.
         * @param token The UDP token to set.
         */
        virtual void setUdpToken(int sessionId, uint64_t token) = 0;

        /**
         * @brief Get UDP token for a session.
         * @param sessionId The ID of the session.
         * @return The UDP token.
         */
        [[nodiscard]] virtual uint64_t getUdpToken(int sessionId) const = 0;

        /**
         * @brief Bind a UDP address to a session.
         * @param sessionId The ID of the session.
         * @param udpAddr The UDP address to bind.
         * @return True if binding was successful, false otherwise.
         */
        [[nodiscard]] virtual bool bindUdp(int sessionId, const sockaddr_in &udpAddr) = 0;

        /**
         * @brief Get the UDP address associated with a session ID.
         * @param sessionId The ID of the session.
         * @return Pointer to the sockaddr_in structure, or nullptr if not found.
         */
        [[nodiscard]] virtual const sockaddr_in *getUdpAddress(int sessionId) const = 0;

        /**
         * @brief Get the session ID associated with a UDP address.
         * @param udpAddr The UDP address of the client.
         * @return The session ID, or -1 if not found.
         */
        [[nodiscard]] virtual int getSessionIdFromUdp(const sockaddr_in &udpAddr) const = 0;

        /**
         * @brief Check if the given sequence number is valid for the session and, if so, update the last seen sequence
         * number.
         * @param sessionId The ID of the session.
         * @param sequence The sequence number to set.
         */
        [[nodiscard]] virtual bool isSequenceValid(int sessionId, uint32_t sequence) const noexcept = 0;

        /**
         * @brief Get the identity associated with a session ID.
         * @param sessionId The ID of the session.
         * @return An optional Identity object.
         */
        [[nodiscard]] virtual std::optional<Auth::Identity> getIdentity(int sessionId) const = 0;

        /**
         * @brief Set the identity for a session.
         * @param sessionId The ID of the session.
         * @param id The Identity object to set.
         * @param ttl Time to live for the identity.
         */
        virtual void setIdentity(int sessionId, Auth::Identity id, std::chrono::seconds ttl) = 0;

        /**
         * @brief Clear the identity associated with a session ID.
         * @param sessionId The ID of the session.
         */
        virtual void clearIdentity(int sessionId) = 0;

        /**
         * @brief Check if a session is authenticated.
         * @param sessionId The ID of the session.
         * @return True if authenticated, false otherwise.
         */
        [[nodiscard]] virtual bool isAuthed(int sessionId) const = 0;

        /**
         * Check if the UDP Addr has tokens left, if he doesn't fuzz, to prevent UDP flood attacks         * @param addr
         * The address of the client.
         * @return True if the packet was consumed, false otherwise.
         */
        [[nodiscard]] virtual bool consumeUdp(const sockaddr_in &addr) = 0;

        /**
         * @brief Set the last score for a session.
         * @param sessionId The ID of the session.
         * @param score The score to set.
         */
        virtual void setLastScore(int sessionId, uint32_t score) = 0;

        /**
         * @brief Get the last score for a session.
         * @param sessionId The ID of the session.
         * @return An optional uint32_t representing the last score.
         */
        [[nodiscard]]
        virtual std::optional<uint32_t> getLastScore(int sessionId) const = 0;

        /**
         * @brief Get the username associated with a session ID.
         * @param sessionId The ID of the session.
         * @return The username as a string.
         */
        [[nodiscard]] virtual std::string getUsername(int sessionId) const = 0;

        /**
         * @brief Find a session ID by username.
         * @param username The username to search for.
         * @return An optional containing the session ID if found, otherwise std::nullopt.
         */
        [[nodiscard]] virtual std::optional<int> findSessionIdByUsername(const std::string &username) const = 0;

        /**
         * @brief Ban an IP address (sin_addr.s_addr format).
         * @param ip IPv4 address in network byte order.
         * @param duration Duration of the ban. If <= 0, defaults to 24h.
         */
        virtual void banIp(uint32_t ip, std::chrono::seconds duration) = 0;

        /**
         * @brief Remove a ban for an IP.
         * @param ip IPv4 address in network byte order.
         */
        virtual void unbanIp(uint32_t ip) = 0;

        /**
         * @brief Check if an IP is currently banned.
         * @param ip IPv4 address in network byte order.
         */
        [[nodiscard]] virtual bool isIpBanned(uint32_t ip) const = 0;

        /**
         * @brief Get a list of banned IPs and their remaining ban durations.
         * @return A vector of pairs containing the banned IP and remaining seconds of the ban.
         */
        [[nodiscard]] virtual std::vector<std::pair<uint32_t, uint64_t>> listBans() const = 0;
    };
} // namespace Net::Server
