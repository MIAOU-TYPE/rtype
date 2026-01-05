#pragma once

#include <iostream>
#include <string>

namespace Utils
{
    /**
     * @enum ArgParseResult
     * @brief Enumeration representing the result of argument parsing.
     */
    enum class ArgParseResult {
        Success = 0,       ///> Parsing was successful
        HelpDisplayed = 1, ///> Help message was displayed
        Error = 84         ///> An error occurred during parsing
    };

    /**
     * @class ParserError
     * @brief Exception class for argument parsing errors.
     * Thrown when an error occurs during argument parsing.
     */
    class ParserError : public std::exception {
      public:
        /**
         * @brief Constructor for ParserError.
         * @param message The error message.
         */
        explicit ParserError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Override of what() method from std::exception.
         * @return The error message.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @class ArgParser
     * @brief Class for parsing command-line arguments.
     */
    class ArgParser {
      public:
        /**
         * @brief Constructor for ArgParser.
         * @param argc Integer representing the number of command-line arguments.
         * @param argv Array of C-strings representing the command-line arguments.
         */
        ArgParser(int argc, char **argv);

        /**
         * @brief Parses the command-line arguments.
         * @return An ArgParseResult indicating the outcome of the parsing.
         */
        [[nodiscard]] ArgParseResult parse() noexcept;

        /**
         * @brief Gets the parsed port number.
         * @return The port number.
         */
        [[nodiscard]] int getPort() const noexcept;

        /**
         * @brief Gets the parsed host address.
         * @return The host address as a string.
         */
        [[nodiscard]] const std::string &getHost() const noexcept;

      private:
        /**
         * @brief Displays the help message.
         */
        void displayHelp() const noexcept;

        /**
         * @brief Parses the port number from a string.
         * @param value The string representing the port number.
         * @return True if parsing was successful, false otherwise.
         */
        [[nodiscard]] bool parsePort(const std::string &value) noexcept;

        /**
         * @brief Parses the host address from a string.
         * @param value The string representing the host address.
         * @return True if parsing was successful, false otherwise.
         */
        [[nodiscard]] bool parseHost(const std::string &value) noexcept;

        int _argc;    ///> Number of command-line arguments
        char **_argv; ///> Array of command-line arguments

        std::string _host = "127.0.0.1"; ///> Default host address
        int _port = 8080;                ///> Default port number
    };
} // namespace Utils
