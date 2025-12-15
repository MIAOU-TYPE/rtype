/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ArgParser
*/
#include "ArgParser.hpp"

#include <iostream>
#include <stdexcept>
using namespace Utils;

ArgParser::ArgParser(const int argc, char **argv) : _argc(argc), _argv(argv)
{
}

ArgParseResult ArgParser::parse()
{
    if (_argc == 1)
        return ArgParseResult::Success;

    if (_argc == 2) {
        const std::string arg = _argv[1];
        if (arg == "-h" || arg == "--help") {
            displayHelp();
            return ArgParseResult::HelpDisplayed;
        }
        return ArgParseResult::Error;
    }

    if (_argc == 3) {
        const std::string flag = _argv[1];
        const std::string value = _argv[2];

        if (flag == "--port") {
            if (!parsePort(value))
                return ArgParseResult::Error;
            return ArgParseResult::Success;
        }
    }

    return ArgParseResult::Error;
}

int ArgParser::getPort() const
{
    return _port;
}

void ArgParser::displayHelp() const
{
    std::cout << "[USAGE]: " << _argv[0] << "\n"
              << "\t--help\t\tDisplay this help message.\n"
              << "\t--port <port>\tSpecify the port number. (default 8080)\n";
}

bool ArgParser::parsePort(const std::string &value)
{
    try {
        int port = std::stoi(value);
        if (port <= 0 || port > 65535)
            throw ParserError("Invalid port");
        _port = port;
        return true;
    } catch (...) {
        std::cerr << "{ArgParser}: Invalid port number." << std::endl;
        return false;
    }
}
