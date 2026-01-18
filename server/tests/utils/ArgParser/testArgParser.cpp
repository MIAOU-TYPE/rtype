/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** testArgParser
*/

#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "ArgParser.hpp"


namespace
{
    struct CaptureStdout {
        CaptureStdout() : _old(std::cout.rdbuf(_ss.rdbuf()))
        {
        }

        ~CaptureStdout()
        {
            std::cout.rdbuf(_old);
        }

        std::string str() const
        {
            return _ss.str();
        }

      private:
        std::stringstream _ss;
        std::streambuf *_old = nullptr;
    };

    struct CaptureStderr {
        CaptureStderr() : _old(std::cerr.rdbuf(_ss.rdbuf()))
        {
        }

        ~CaptureStderr()
        {
            std::cerr.rdbuf(_old);
        }

        std::string str() const
        {
            return _ss.str();
        }

      private:
        std::stringstream _ss;
        std::streambuf *_old = nullptr;
    };

    struct ArgvBuilder {
        explicit ArgvBuilder(std::vector<std::string> args) : _args(std::move(args))
        {
            _argv.reserve(_args.size());
            for (auto &s : _args)
                _argv.push_back(s.data());
        }

        int argc() const
        {
            return static_cast<int>(_argv.size());
        }

        char **argv()
        {
            return _argv.data();
        }

      private:
        std::vector<std::string> _args;
        std::vector<char *> _argv;
    };
}

TEST(ArgParser, DefaultNoArgsSuccessAndKeepsDefaults)
{
    ArgvBuilder a({"prog"});
    Utils::ArgParser p(a.argc(), a.argv());

    const auto res = p.parse();
    EXPECT_EQ(res, Utils::ArgParseResult::Success);

    EXPECT_EQ(p.getPort(), 8080);
    EXPECT_EQ(p.getHost(), "127.0.0.1");
}

TEST(ArgParser, HelpShortHPrintsHelpAndReturnsHelpDisplayed)
{
    ArgvBuilder a({"prog", "-h"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStdout out;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::HelpDisplayed);
    const auto s = out.str();
    EXPECT_NE(s.find("[USAGE]:"), std::string::npos);
    EXPECT_NE(s.find("--host"), std::string::npos);
    EXPECT_NE(s.find("--port"), std::string::npos);
    EXPECT_NE(s.find("--help"), std::string::npos);
}

TEST(ArgParser, HelpLongPrintsHelpAndReturnsHelpDisplayed)
{
    ArgvBuilder a({"prog", "--help"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStdout out;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::HelpDisplayed);
    EXPECT_NE(out.str().find("[USAGE]:"), std::string::npos);
}

TEST(ArgParser, PortValidSetsPortAndReturnsSuccess)
{
    ArgvBuilder a({"prog", "--port", "4242"});
    Utils::ArgParser p(a.argc(), a.argv());

    const auto res = p.parse();
    EXPECT_EQ(res, Utils::ArgParseResult::Success);
    EXPECT_EQ(p.getPort(), 4242);
}

TEST(ArgParser, HostValidSetsHostAndReturnsSuccess)
{
    ArgvBuilder a({"prog", "--host", "192.168.0.10"});
    Utils::ArgParser p(a.argc(), a.argv());

    const auto res = p.parse();
    EXPECT_EQ(res, Utils::ArgParseResult::Success);
    EXPECT_EQ(p.getHost(), "192.168.0.10");
}

TEST(ArgParser, HostAndPortTogetherAreParsed)
{
    ArgvBuilder a({"prog", "--host", "10.0.0.1", "--port", "9999"});
    Utils::ArgParser p(a.argc(), a.argv());

    const auto res = p.parse();
    EXPECT_EQ(res, Utils::ArgParseResult::Success);
    EXPECT_EQ(p.getHost(), "10.0.0.1");
    EXPECT_EQ(p.getPort(), 9999);
}

TEST(ArgParser, PortMissingValueReturnsError)
{
    ArgvBuilder a({"prog", "--port"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStderr err;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::Error);
    (void) err;
}

TEST(ArgParser, HostMissingValueReturnsError)
{
    ArgvBuilder a({"prog", "--host"});
    Utils::ArgParser p(a.argc(), a.argv());

    const auto res = p.parse();
    EXPECT_EQ(res, Utils::ArgParseResult::Error);
}

TEST(ArgParser, PortOutOfRangeLowReturnsError)
{
    ArgvBuilder a({"prog", "--port", "0"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStderr err;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::Error);
    EXPECT_NE(err.str().find("between 1 and 65535"), std::string::npos);
}

TEST(ArgParser, PortOutOfRangeHighReturnsError)
{
    ArgvBuilder a({"prog", "--port", "70000"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStderr err;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::Error);
    EXPECT_NE(err.str().find("between 1 and 65535"), std::string::npos);
}

TEST(ArgParser, PortNotANumberReturnsError)
{
    ArgvBuilder a({"prog", "--port", "abc"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStderr err;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::Error);
    EXPECT_NE(err.str().find("Invalid port number"), std::string::npos);
}

TEST(ArgParser, HostEmptyStringReturnsError)
{
    ArgvBuilder a({"prog", "--host", ""});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStderr err;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::Error);
    EXPECT_NE(err.str().find("Invalid host value"), std::string::npos);
}

TEST(ArgParser, UnknownArgumentReturnsError)
{
    ArgvBuilder a({"prog", "--nope"});
    Utils::ArgParser p(a.argc(), a.argv());

    CaptureStderr err;
    const auto res = p.parse();

    EXPECT_EQ(res, Utils::ArgParseResult::Error);
    EXPECT_NE(err.str().find("Unknown argument"), std::string::npos);
}

TEST(ArgParser, StopsOnFirstErrorDoesNotOverridePreviousValidValues)
{
    ArgvBuilder a({"prog", "--port", "4242", "--nope", "--port", "1234"});
    Utils::ArgParser p(a.argc(), a.argv());

    const auto res = p.parse();
    EXPECT_EQ(res, Utils::ArgParseResult::Error);

    EXPECT_EQ(p.getPort(), 4242);
}
