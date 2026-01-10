/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AuthService
*/

#pragma once
#include "Password.hpp"
#include "UserStorage.hpp"

namespace Auth
{
    /**
     * @class AuthServiceError
     * @brief Exception class for authentication service errors.
     */
    class AuthServiceError : public std::exception {
      public:
        /**
         * @brief Constructs an AuthServiceError with a given kind and message.
         * @param kind The kind of error.
         * @param message The error message.
         */
        AuthServiceError(std::string kind, std::string message) : _kind(std::move(kind)), _message(std::move(message))
        {
            _what = _kind + ": " + _message;
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _what.c_str();
        }

        /**
         * @brief Returns the kind of error.
         * @return The kind of error as a string view.
         */
        std::string_view kind() const noexcept
        {
            return _kind;
        }

        /**
         * @brief Returns the error message.
         * @return The error message as a string view.
         */
        std::string_view message() const noexcept
        {
            return _message;
        }

      private:
        std::string _kind;    ///> The kind of error.
        std::string _message; ///> The error message.
        std::string _what;    ///> The combined error message.
    };

    /**
     * @struct AuthOk
     * @brief Structure representing a successful authentication result.
     */
    struct AuthOk {
        uint32_t userId = 0;  ///> User ID
        std::string username; ///> Username
    };

    /**
     * @class AuthService
     * @brief Service class for user authentication and registration.
     */
    class AuthService {
      public:
        /**
         * @brief Constructs an AuthService with the given UserStorage repository.
         * @param repo Shared pointer to the UserStorage repository.
         * @throws AuthServiceError if the repository is null.
         */
        explicit AuthService(std::shared_ptr<UserStorage> repo);

        /**
         * @brief Registers a new user with the given username and password.
         * @param username The username of the new user.
         * @param password The password of the new user.
         * @return AuthOk structure containing the user ID and username.
         * @throws AuthServiceError on validation or database errors.
         */
        [[nodiscard]] AuthOk registerUser(const std::string &username, const std::string &password) const;

        /**
         * @brief Logs in a user with the given username and password.
         * @param username The username of the user.
         * @param password The password of the user.
         * @return AuthOk structure containing the user ID and username.
         * @throws AuthServiceError on validation or database errors.
         */
        [[nodiscard]] AuthOk login(const std::string &username, const std::string &password) const;

      private:
        std::shared_ptr<UserStorage> _repo; ///> UserStorage repository

        static bool validUsername(const std::string &u); ///> Validate username
        static bool validPassword(const std::string &p); ///> Validate password
    };
} // namespace Auth
