/*
** EPITECH PROJECT, 2025
** ScoreService.hpp
** File description:
** ScoreService
*/

#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>
#include <exception>
#include <sqlite3.h>
#include "ScoreEntry.hpp"
#include "SqliteDb.hpp"
#include "SqliteStatement.hpp"

namespace Engine
{
    /**
     * @class ScoreServiceError
     * @brief Exception class for ScoreService errors.
     */
    class ScoreServiceError : public std::exception {
    public:
        /**
         * @brief Construct a new Score Service Error object
         * @param message The error message.
         */
        explicit ScoreServiceError(std::string  message) : _message(std::move(message)) {}

        /**
         * @brief Get the error message.
         * @return const char* The error message.
         */
        const char* what() const noexcept override {
            return _message.c_str();
        }
    private:
        std::string _message;   ///> The error message.
    };

    /**
     * @class ScoreService
     * @brief Service for managing player scores in the database.
     */
    class ScoreService {
    public:
        /**
         * @brief Construct a new Score Service object
         *
         * @param db Shared pointer to the SqliteDb instance.
         * @throws std::runtime_error if db is null.
         */
        explicit ScoreService(std::shared_ptr<Db::SqliteDb> db);

        /**
         * @brief Initialize the database schema for scores.
         *
         * Creates the necessary tables and indexes if they do not exist.
         */
        void initSchema() const;

        /**
         * @brief Save a player's score to the database.
         *
         * @param username The username of the player.
         * @param score The score to save.
         * @throws std::runtime_error on database errors.
         */
        void saveScore(const std::string& username, int score) const;

        /**
         * @brief Retrieve the top scores from the database.
         *
         * @param limit The maximum number of top scores to retrieve. Default is 10.
         * @return std::vector<ScoreEntry> A vector of ScoreEntry objects representing the top scores.
         * @throws std::runtime_error on database errors.
         */
        std::vector<ScoreEntry> getTopScores(std::size_t limit = 10) const;

    private:
        std::shared_ptr<Db::SqliteDb> _db;  ///> Shared pointer to the database instance.
    };
}
