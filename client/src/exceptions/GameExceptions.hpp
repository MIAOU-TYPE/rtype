/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameExceptions
*/

#pragma once

#include <stdexcept>
#include <string>

/**
 * @class GameException
 * @brief Base exception class for game-related errors.
 *
 * This class serves as the base for all game-specific exceptions,
 * providing a consistent interface for error handling.
 */
class GameException : public std::exception {
  protected:
    std::string message; ///< The error message

  public:
    /**
     * @brief Constructor for GameException.
     * @param msg The error message.
     */
    explicit GameException(const std::string &msg) : message(msg)
    {
    }

    /**
     * @brief Returns the error message.
     * @return The error message as a C-string.
     */
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

/**
 * @class InitializationException
 * @brief Exception thrown during game initialization failures.
 *
 * This exception is thrown when the game client fails to initialize
 * its components (renderer, input handler, etc.).
 */
class InitializationException : public GameException {
  public:
    /**
     * @brief Constructor for InitializationException.
     * @param component The component that failed to initialize.
     * @param reason The reason for the failure.
     */
    InitializationException(const std::string &component, const std::string &reason)
        : GameException("Failed to initialize " + component + ": " + reason)
    {
    }
};

/**
 * @class RenderException
 * @brief Exception thrown during rendering operations.
 *
 * This exception is thrown when rendering operations fail
 * (window creation, drawing, etc.).
 */
class RenderException : public GameException {
  public:
    /**
     * @brief Constructor for RenderException.
     * @param operation The rendering operation that failed.
     * @param reason The reason for the failure.
     */
    RenderException(const std::string &operation, const std::string &reason)
        : GameException("Rendering error during " + operation + ": " + reason)
    {
    }
};

/**
 * @class InputException
 * @brief Exception thrown during input handling operations.
 *
 * This exception is thrown when input handling operations fail.
 */
class InputException : public GameException {
  public:
    /**
     * @brief Constructor for InputException.
     * @param operation The input operation that failed.
     * @param reason The reason for the failure.
     */
    InputException(const std::string &operation, const std::string &reason)
        : GameException("Input error during " + operation + ": " + reason)
    {
    }
};

/**
 * @class GameLoopException
 * @brief Exception thrown during game loop execution.
 *
 * This exception is thrown when the main game loop encounters
 * an unrecoverable error.
 */
class GameLoopException : public GameException {
  public:
    /**
     * @brief Constructor for GameLoopException.
     * @param reason The reason for the game loop failure.
     */
    explicit GameLoopException(const std::string &reason) : GameException("Game loop error: " + reason)
    {
    }
};
