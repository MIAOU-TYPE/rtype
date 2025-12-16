#include "ArgParser.hpp"
#include <iostream>

using namespace Utils;

ArgParser::ArgParser(const int argc, char **argv) : _argc(argc), _argv(argv)
{
}

ArgParseResult ArgParser::parse()
{
    for (int i = 1; i < _argc; i++) {
        const std::string arg = _argv[i];

        if (arg == "-h" || arg == "--help") {
            displayHelp();
            return ArgParseResult::HelpDisplayed;
        }

        if (arg == "--port") {
            if (i + 1 >= _argc || !parsePort(_argv[++i]))
                return ArgParseResult::Error;
            continue;
        }

        if (arg == "--host") {
            if (i + 1 >= _argc || !parseHost(_argv[++i]))
                return ArgParseResult::Error;
            continue;
        }

        std::cerr << "{ArgParser}: Unknown argument: " << arg << std::endl;
        return ArgParseResult::Error;
    }
    return ArgParseResult::Success;
}

int ArgParser::getPort() const
{
    return _port;
}

const std::string &ArgParser::getHost() const
{
    return _host;
}

void ArgParser::displayHelp() const
{
    std::cout << "[USAGE]: " << _argv[0] << "\n\n"
              << "Options:\n"
              << "  --host <ip>     Server IP address (default: 127.0.0.1)\n"
              << "  --port <port>   Server port (default: 8080)\n"
              << "  -h, --help      Display this help message\n";
}

bool ArgParser::parsePort(const std::string &value)
{
    try {
        const int port = std::stoi(value);

        if (port <= 0 || port > 65535)
            throw ParserError("Invalid port range");
        _port = port;
        return true;
    } catch (...) {
        std::cerr << "{ArgParser}: Invalid port number." << std::endl;
        return false;
    }
}

bool ArgParser::parseHost(const std::string &value)
{
    if (value.empty()) {
        std::cerr << "{ArgParser}: Invalid host value." << std::endl;
        return false;
    }
    _host = value;
    return true;
}
