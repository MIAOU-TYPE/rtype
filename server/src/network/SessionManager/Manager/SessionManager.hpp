/*
** EPITECH PROJECT, 2025
** RType
** File description:
** SessionManager
*/

#pragma once
#include <chrono>
#include <cstdint>
#include <mutex>
#include <optional>
#include "Endian.hpp"
#include "ISessionManager.hpp"
#include "UserStorage.hpp"
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

        /**
         * @brief Check if the given sequence number is valid for the session and, if so, update the last seen sequence
         * number.
         * @param sessionId The ID of the session.
         * @param sequence The sequence number to set.
         */
        [[nodiscard]] bool isSequenceValid(int sessionId, uint32_t sequence) const noexcept override;

        /**
         * @brief Clear the identity associated with a session ID.
         * @param sessionId The ID of the session.
         */
        void clearIdentity(int sessionId) override;

        /**
         * @brief Set the identity for a session ID with a time-to-live (TTL).
         * @param sessionId The ID of the session.
         * @param id The identity to set.
         * @param ttl The time-to-live duration.
         */
        void setIdentity(int sessionId, Auth::Identity id, std::chrono::seconds ttl) override;

        /**
         * @brief Get the identity associated with a session ID.
         * @param sessionId The ID of the session.
         * @return An optional containing the identity if it exists, otherwise std::nullopt.
         */
        [[nodiscard]] std::optional<Auth::Identity> getIdentity(int sessionId) const override;

        /**
         * @brief Check if a session ID is authenticated.
         * @param sessionId The ID of the session.
         * @return True if the session is authenticated, false otherwise.
         */
        [[nodiscard]] bool isAuthed(int sessionId) const override;

        /**
         * @brief Check if the UDP Addr has tokens left, if he doesn't fuzz, to prevent UDP flood attacks
         * @param addr The address from which the UDP packet was received.
         * @return True if the packet was successfully consumed, false otherwise.
         */
        [[nodiscard]] bool consumeUdp(const sockaddr_in &addr) override;

        /**
         * @brief Set the last score for a session ID.
         * @param sessionId The ID of the session.
         * @param score The score to set.
         */
        void setLastScore(int sessionId, uint32_t score) override;

        /**
         * @brief Get the last score associated with a session ID.
         * @param sessionId The ID of the session.
         * @return An optional containing the last score if it exists, otherwise std::nullopt.
         */
        [[nodiscard]]
        std::optional<uint32_t> getLastScore(int sessionId) const override;

        /**
         * @brief Get the username associated with a session ID.
         * @param sessionId The ID of the session.
         * @return The username as a string.
         */
        [[nodiscard]] std::string getUsername(int sessionId) const override;

        /**
         * @brief Find a session ID by username.
         * @param username The username to search for.
         * @return An optional containing the session ID if found, otherwise std::nullopt.
         */
        [[nodiscard]] std::optional<int> findSessionIdByUsername(const std::string &username) const override;

        /**
         * @brief Ban an IP address (sin_addr.s_addr format).
         * @param ip IPv4 address in network byte order.
         * @param duration Duration of the ban. If <= 0, defaults to 24h.
         */
        void banIp(uint32_t ip, std::chrono::seconds duration) override;

        /**
         * @brief Remove a ban for an IP.
         * @param ip IPv4 address in network byte order.
         */
        void unbanIp(uint32_t ip) override;

        /**
         * @brief Check if an IP is currently banned.
         * @param ip IPv4 address in network byte order.
         */
        [[nodiscard]] bool isIpBanned(uint32_t ip) const override;

        /**
         * @brief Snapshot of banned IPs and remaining seconds.
         */
        [[nodiscard]] std::vector<std::pair<uint32_t, uint64_t>> listBans() const override;

      private:
        mutable std::shared_mutex _mutex{};      ///> Mutex for thread-safe access
        using Clock = std::chrono::steady_clock; ///> Clock type for time management

        /**
         * @brief Clear the authentication for a session ID (locked version).
         * @param sessionId The ID of the session.
         */
        void clearAuthLocked(int sessionId);

        /**
         * @brief Check if the authentication for a session ID has expired.
         * @param sessionId The ID of the session.
         * @return True if the authentication has expired, false otherwise.
         */
        [[nodiscard]] bool isExpiredLocked(int sessionId) const;

        /**
         * @brief Clean up expired bans from the banned IP list.
         */
        void cleanupBansLocked() const;

        std::unordered_map<int, Auth::Identity> _identityById{};      ///> Identity storage
        std::unordered_map<int, Clock::time_point> _authExpiryById{}; ///> Authentication expiry storage

        std::unordered_map<AddressKey, int, AddressKeyHash> _tcpAddressToId{}; ///> TCP legacy binding
        std::unordered_map<int, sockaddr_in> _idToTcpAddress{};                ///> TCP legacy binding

        std::unordered_map<AddressKey, int, AddressKeyHash> _udpAddressToId{}; ///> UDP binding
        std::unordered_map<int, sockaddr_in> _idToUdpAddress{};                ///> UDP binding

        std::unordered_map<int, uint64_t> _udpTokenById{};             ///> UDP token storage
        mutable std::unordered_map<int, uint32_t> _lastSequenceById{}; ///> Last UDP sequence storage

        std::unordered_map<int, uint32_t> _lastScoreById{}; ///> Last score storage

        mutable std::unordered_map<uint32_t, Clock::time_point> _bannedIpUntil{}; ///> Banned IPs storage
        int _nextId = 1;                                                          ///> Next available session ID

        /**
         * @brief Structure to manage UDP rate limiting tokens and timestamps.
         */
        struct UdpRate {
            uint32_t tokens; ///> Available tokens
            uint64_t lastNs; ///> Timestamp of the last token update in nanoseconds
        };

        std::unordered_map<AddressKey, UdpRate, AddressKeyHash> _udpRates; ///> UDP rate limiting data
        static constexpr uint32_t MaxTokens = 60;                          ///> Maximum tokens for rate limiting
        static constexpr uint32_t TokenRefillRateNs = 16'000'000;          ///> Token refill rate in nanoseconds
    };
} // namespace Net::Server
