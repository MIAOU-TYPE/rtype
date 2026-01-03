/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager
*/

#pragma once

#include <cstdint>
#include <mutex>
#include "Endian.hpp"
#include "ISessionManager.hpp"
#include <shared_mutex>

#include <unordered_map>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netinet/in.h>
#endif

/**
 * @brief A key representing a network address (IP and port).
 */
namespace Net::Server
{
    /**
     * @brief Manages network sessions by mapping addresses to session IDs.
     * Provides thread-safe operations to create, retrieve, and remove sessions.
     */
    class SessionManager : public ISessionManager {
      public:
        /**
         * @brief Get an existing session ID for the given address or create a new one.
         * @param address The network address.
         * @return The session ID associated with the address.
         */
        [[nodiscard]] int getOrCreateSession(const sockaddr_in &address) override;

        /**
         * @brief Get the session ID for the given address.
         * @param address The network address.
         * @return The session ID if it exists, otherwise -1.
         */
        [[nodiscard]] int getSessionId(const sockaddr_in &address) const override;

        /**
         * @brief Remove the session associated with the given session ID.
         * @param sessionId The session ID to remove.
         */
        void removeSession(int sessionId) override;

        /**
         * @brief Get the address associated with the given session ID.
         * @param sessionId The session ID.
         * @return A pointer to the sockaddr_in if found, otherwise nullptr.
         */
        [[nodiscard]] const sockaddr_in *getAddress(int sessionId) const override;

        /**
         * @brief Get all active sessions.
         * @return A vector of pairs containing session IDs and their corresponding addresses.
         */
        [[nodiscard]] std::vector<std::pair<int, sockaddr_in>> getAllSessions() const override;

        /**
         * @brief Apply a function to each session.
         * @param func The function to apply, taking session ID and address as parameters.
         */
        void forEachSession(const std::function<void(int, const sockaddr_in &)> &func) const override;

        /**
         * @brief Set the UDP token for a session.
         * @param sessionId The ID of the session.
         * @param token The UDP token to set.
         */
        void setUdpToken(int sessionId, uint64_t token) override;

        /**
         * @brief Get the UDP token for a session.
         * @param sessionId The ID of the session.
         * @return The UDP token associated with the session.
         */
        [[nodiscard]] uint64_t getUdpToken(int sessionId) const override;

        /**
         * @brief Bind a UDP address to a session ID.
         * @param sessionId The ID of the session.
         * @param udpAddr The UDP address to bind.
         * @return True if the binding was successful, false otherwise.
         */
        [[nodiscard]] bool bindUdp(int sessionId, const sockaddr_in &udpAddr) override;

        /**
         * @brief Get the UDP address associated with a session ID.
         * @param sessionId The ID of the session.
         * @return A pointer to the sockaddr_in if found, otherwise nullptr.
         */
        [[nodiscard]] const sockaddr_in *getUdpAddress(int sessionId) const override;

        /**
         * @brief Get the session ID associated with a UDP address.
         * @param udpAddr The UDP address.
         * @return The session ID if it exists, otherwise -1.
         */
        [[nodiscard]] int getSessionIdFromUdp(const sockaddr_in &udpAddr) const override;

      private:
        mutable std::shared_mutex _mutex{}; ///> Mutex for thread-safe access

        std::unordered_map<AddressKey, int, AddressKeyHash> _tcpAddressToId{}; ///> TCP legacy binding
        std::unordered_map<int, sockaddr_in> _idToTcpAddress{};                ///> TCP legacy binding

        std::unordered_map<AddressKey, int, AddressKeyHash> _udpAddressToId{}; ///> UDP binding
        std::unordered_map<int, sockaddr_in> _idToUdpAddress{};                ///> UDP binding

        std::unordered_map<int, uint64_t> _udpTokenById{}; ///> UDP token storage

        int _nextId = 1; ///> Next available session ID
    };
} // namespace Net::Server
