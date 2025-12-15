/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** argParser
*/

#pragma once

#include <string>

namespace Utils
{
    /**
     * @enum ArgParseResult
     * @brief Enumeration for the result of argument parsing.
     */
    enum class ArgParseResult {
        Success = 0,       ///> Parsing was successful
        HelpDisplayed = 1, ///> Help information was displayed
        Error = 84         ///> An error occurred during parsing
    };

    /*
     * @class ParserError
     * @brief Exception class for argument parser errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class ParserError : public std::exception {
      public:
        /**
         * @brief Constructor for ParserError.
         * @param message The error message to be associated with the exception.
         */
        explicit ParserError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @brief Command-line argument parser for the server application.
     * Parses command-line arguments to extract configuration options such as port number.
     * Supports displaying help information.
     */
    class ArgParser {
      public:
        /**
         * @brief Constructor for ArgParser.
         * @param argc
         * @param argv
         */
        ArgParser(int argc, char **argv);

        /**
         * @brief Parses the command-line arguments.
         * @return An ArgParseResult indicating the outcome of the parsing.
         */
        ArgParseResult parse();

        /**
         * @brief Retrieves the parsed port number.
         * @return The port number specified in the command-line arguments, or the default if not specified.
         */
        int getPort() const;

      private:
        /**
         * @brief Displays help information for command-line arguments.
         */
        void displayHelp() const;

        /**
         * @brief Parses and validates the port number from a string.
         * @param value The string representation of the port number.
         * @return True if the port number is valid and successfully parsed, false otherwise.
         */
        bool parsePort(const std::string &value);

        int _argc = 0;          ///> Argument count
        char **_argv = nullptr; ///> Argument vector
        int _port = 8080;       ///> Default port number
    };
} // namespace Utils