/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SqliteDb
*/

#pragma once
#include <exception>
#include <mutex>
#include <sqlite3.h>
#include <string>
#include <utility>
#include <string_view>

namespace Db
{
    /**
     * @brief Exception thrown on SQLite database errors
     */
    class SqliteDbError : public std::exception {
      public:
        /**
         * @brief Construct a new Sqlite Db Error object
         * @param message Error message
         */
        explicit SqliteDbError(std::string message) : _message(std::move(message))
        {
        }

        /**
         * @brief Get the error message
         * @return const char* Error message
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief SQLite database wrapper
     */
    class SqliteDb {
      public:
        /**
         * @brief Construct a new Sqlite Db object
         * @param path Path to the SQLite database file
         * @throws SqliteDbError on failure
         */
        explicit SqliteDb(const std::string &path);
        /**
         * @brief Destroy the Sqlite Db object
         */
        ~SqliteDb();

        /**
         * @brief Delete copy constructor
         */
        SqliteDb(const SqliteDb &) = delete;

        /**
         * @brief Delete copy assignment operator
         */
        SqliteDb &operator=(const SqliteDb &) = delete;

        /**
         * @brief Execute a SQL statement without locking
         * @param sql SQL statement to execute
         * @throws SqliteDbError on failure
         */
        void execUnlocked(std::string_view sql) const;

        /**
         * @brief Get the raw sqlite3 pointer
         * @return sqlite3* Raw sqlite3 pointer
         */
        [[nodiscard]] sqlite3 *raw() const noexcept;

        /**
         * @brief Get the mutex for thread-safe operations
         * @return std::mutex& Reference to the mutex
         */
        [[nodiscard]] std::mutex &mutex() const noexcept;

        /**
         * @brief Execute a SQL statement
         * @param sql SQL statement to execute
         * @throws SqliteDbError on failure
         */
        void exec(std::string_view sql) const;

      private:
        sqlite3 *_db = nullptr;  ///> Raw sqlite3 pointer
        mutable std::mutex _mtx; ///> Mutex for thread-safe operations
    };
} // namespace Auth
