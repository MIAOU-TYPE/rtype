/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Password
*/

#pragma once

#include <array>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <utility>
#include <string_view>

namespace Auth
{
    /**
     * @class PasswordError
     * @brief Exception class for password-related errors.
     */
    class PasswordError : public std::exception {
      public:
        /**
         * @brief Constructs a PasswordError with a given message.
         * @param message The error message.
         */
        explicit PasswordError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> The error message.
    };

    /**
     * @namespace
     * @brief Internal helper functions for password hashing and verification.
     */
    [[nodiscard]] std::string hashPassword(std::string_view password);

    /**
     * @brief Verify a password against an encoded hash.
     * @param password The plaintext password to verify.
     * @param encoded The encoded hash to verify against.
     * @return true if the password matches the hash, false otherwise.
     */
    [[nodiscard]] bool verifyPassword(std::string_view password, std::string_view encoded) noexcept;
} // namespace Auth

#include "Password.tpp"