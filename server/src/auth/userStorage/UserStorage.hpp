/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UserStorage
*/

#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "SqliteDb.hpp"
#include "SqliteStatement.hpp"

namespace Auth
{
    /**
     * @class UserStorageError
     * @brief Exception class for UserStorage-related errors.
     */
    class UserStorageError : public std::exception {
      public:
        /**
         * @brief Constructs a UserStorageError with a given message.
         * @param message The error message.
         */
        explicit UserStorageError(std::string message) : _message(std::move(message))
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

    struct Identity {
        uint32_t userId = 0;  ///> The user ID.
        std::string username; ///> The username.
    };

    /**
     * @struct UserRow
     * @brief Represents a user record in the database.
     */
    struct UserRow {
        Identity identity;  ///> The user's identity information.
        std::string passwordScrypt; ///> The hashed password.
    };

    /**
     * @class UserStorage
     * @brief Manages user data storage and retrieval in the database.
     */
    class UserStorage {
      public:
        /**
         * @brief Constructs a UserStorage with a given database connection.
         * @param db A shared pointer to the SqliteDb instance.
         * @throws UserStorageError if the provided database pointer is null.
         */
        explicit UserStorage(std::shared_ptr<SqliteDb> db);

        /**
         * @brief Initializes the database schema for user storage.
         * @throws UserStorageError on database errors.
         */
        void initSchema() const;

        /**
         * @brief Finds a user by their username.
         * @param username The username to search for.
         * @return An optional UserRow containing the user data if found, or std::nullopt if not found.
         * @throws UserStorageError on database errors.
         */
        [[nodiscard]] std::optional<UserRow> findByUsername(const std::string &username) const;

        /**
         * @brief Inserts a new user into the database.
         * @param username The username of the new user.
         * @param passwordScrypt The hashed password of the new user.
         * @return The ID of the newly inserted user.
         * @throws UserStorageError on database errors.
         */
        [[nodiscard]] uint32_t insertUser(const std::string &username, const std::string &passwordScrypt) const;

      private:
        std::shared_ptr<SqliteDb> _db; ///> The database connection.
    };
} // namespace Auth
