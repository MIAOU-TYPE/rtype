/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SqliteStatement
*/

#pragma once
#include <exception>
#include <sqlite3.h>
#include <string>
#include <utility>

namespace Db
{
    /**
     * @class SqlStatementError
     * @brief Exception class for SQLite statement errors.
     */
    class SqlStatementError : public std::exception {
      public:
        /**
         * @brief Constructs a SqlStatementError with a given message.
         * @param message The error message.
         */
        explicit SqlStatementError(std::string message) : _message(std::move(message))
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
     * @class SqlStatement
     * @brief Wrapper class for SQLite prepared statements.
     */
    class SqlStatement {
      public:
        /**
         * @brief Constructs a SqlStatement by preparing the given SQL statement.
         * @param db Pointer to the SQLite database.
         * @param sql The SQL statement to prepare.
         * @throws SqlStatementError if preparation fails.
         */
        SqlStatement(sqlite3 *db, const char *sql);

        /**
         * @brief Destructor that finalizes the prepared statement.
         */
        ~SqlStatement();

        /**
         * @brief Deleted copy constructor to prevent copying.
         */
        SqlStatement(const SqlStatement &) = delete;

        /**
         * @brief Deleted copy assignment operator to prevent copying.
         */
        SqlStatement &operator=(const SqlStatement &) = delete;

        /**
         * @brief Returns the raw SQLite statement pointer.
         * @return Pointer to the SQLite prepared statement.
         */
        [[nodiscard]] sqlite3_stmt *raw() const noexcept;

      private:
        sqlite3_stmt *_statement = nullptr; ///> Pointer to the prepared statement.
    };
} // namespace Auth
