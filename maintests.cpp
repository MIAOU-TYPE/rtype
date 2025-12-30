/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** maintests
*/

// main.cpp - NetWrapper full smoke test
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <errno.h>
#endif

#include "NetWrapper.hpp"

static void ok(const char *label)
{
    std::cout << "[OK]   " << label << "\n";
}

static void fail(const char *label, const std::string &msg = {})
{
    std::cerr << "[FAIL] " << label;
    if (!msg.empty())
        std::cerr << " - " << msg;
    std::cerr << "\n";
}

static uint16_t getBoundPort(socketHandle s)
{
    sockaddr_in addr {};
#ifdef _WIN32
    int len = sizeof(addr);
#else
    socklen_t len = sizeof(addr);
#endif
    if (::getsockname(s, reinterpret_cast<sockaddr *>(&addr), &len) != 0)
        return 0;
    return ntohs(addr.sin_port);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <pluginPath> [baseDir]\n";
        return 2;
    }

    const std::string pluginPath = argv[1];
    const std::string baseDir    = (argc >= 3) ? argv[2] : ".";

    try {
        Net::NetWrapper net(pluginPath, baseDir);

        // -------- initNetwork / cleanupNetwork --------
        if (net.initNetwork() != 0) {
            fail("initNetwork()", "returned non-zero");
            return 1;
        }
        ok("initNetwork()");

        // ============================================================
        // UDP TEST: socket + bind + sendTo + recvFrom + closeSocket
        // ============================================================
        socketHandle udpA = net.socket(AF_INET, SOCK_DGRAM, 0);
        socketHandle udpB = net.socket(AF_INET, SOCK_DGRAM, 0);
        if (udpA == kInvalidSocket || udpB == kInvalidSocket) {
            fail("socket(AF_INET, SOCK_DGRAM)", "invalid socket");
            return 1;
        }
        ok("socket(AF_INET, SOCK_DGRAM)");

        sockaddr_in aAddr {};
        aAddr.sin_family = AF_INET;
        aAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        aAddr.sin_port = htons(0);

        sockaddr_in bAddr {};
        bAddr.sin_family = AF_INET;
        bAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        bAddr.sin_port = htons(0);

        if (net.bind(udpA, reinterpret_cast<const sockaddr *>(&aAddr), sizeof(aAddr)) != 0 ||
            net.bind(udpB, reinterpret_cast<const sockaddr *>(&bAddr), sizeof(bAddr)) != 0) {
            fail("bind(UDP)", "bind failed");
            return 1;
        }
        ok("bind(UDP)");

        const uint16_t udpBPort = getBoundPort(udpB);
        if (udpBPort == 0) {
            fail("getsockname(UDP)", "port=0");
            return 1;
        }
        bAddr.sin_port = htons(udpBPort);

        // setSocketOpt (just to call it at least once)
        int yes = 1;
        if (net.setSocketOpt(udpA, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0) {
            // SO_REUSEADDR can behave differently on some platforms; don't hard-fail
            fail("setSocketOpt(SO_REUSEADDR) on UDP", "non-zero (continuing)");
        } else {
            ok("setSocketOpt(SO_REUSEADDR) on UDP");
        }

        const char udpMsg[] = "udp_smoke";
        auto sUdp = net.sendTo(
            udpA, udpMsg, sizeof(udpMsg), 0, reinterpret_cast<const sockaddr *>(&bAddr), sizeof(bAddr));
        if (sUdp <= 0) {
            fail("sendTo()", "returned <= 0");
            return 1;
        }
        ok("sendTo()");

        char udpBuf[256] {};
        sockaddr_in from {};
#ifdef _WIN32
        int fromLen = sizeof(from);
#else
        socklen_t fromLen = sizeof(from);
#endif
        auto rUdp = net.recvFrom(
            udpB, udpBuf, sizeof(udpBuf), 0, reinterpret_cast<sockaddr *>(&from), &fromLen);
        if (rUdp <= 0) {
            fail("recvFrom()", "returned <= 0");
            return 1;
        }
        ok("recvFrom()");
        std::cout << "UDP received: " << udpBuf << "\n";

        net.closeSocket(udpA);
        net.closeSocket(udpB);
        ok("closeSocket(UDP)");

        // ============================================================
        // TCP TEST: socket + setOpt + bind + listen + accept + connect + send/recv + closeSocket
        // ============================================================
        socketHandle server = net.socket(AF_INET, SOCK_STREAM, 0);
        if (server == kInvalidSocket) {
            fail("socket(AF_INET, SOCK_STREAM)", "invalid socket");
            return 1;
        }
        ok("socket(AF_INET, SOCK_STREAM)");

        // setSocketOpt: SO_REUSEADDR
        if (net.setSocketOpt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0) {
            fail("setSocketOpt(SO_REUSEADDR) on TCP", "non-zero (continuing)");
        } else {
            ok("setSocketOpt(SO_REUSEADDR) on TCP");
        }

        sockaddr_in sAddr {};
        sAddr.sin_family = AF_INET;
        sAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        sAddr.sin_port = htons(0);

        if (net.bind(server, reinterpret_cast<const sockaddr *>(&sAddr), sizeof(sAddr)) != 0) {
            fail("bind(TCP server)", "bind failed");
            return 1;
        }
        ok("bind(TCP server)");

        const uint16_t tcpPort = getBoundPort(server);
        if (tcpPort == 0) {
            fail("getsockname(TCP server)", "port=0");
            return 1;
        }

        if (net.listen(server, 8) != 0) {
            fail("listen()", "listen failed");
            return 1;
        }
        ok("listen()");

        // accept in a thread (blocking)
        std::promise<socketHandle> acceptedPromise;
        auto acceptedFuture = acceptedPromise.get_future();
        std::thread acceptThread([&] {
            sockaddr_in cAddr {};
#ifdef _WIN32
            int len = sizeof(cAddr);
#else
            socklen_t len = sizeof(cAddr);
#endif
            socketHandle c = net.accept(server, reinterpret_cast<sockaddr *>(&cAddr), &len);
            acceptedPromise.set_value(c);
        });

        // client socket + connect (wrapper connect)
        socketHandle client = net.socket(AF_INET, SOCK_STREAM, 0);
        if (client == kInvalidSocket) {
            fail("socket(TCP client)", "invalid socket");
            return 1;
        }
        ok("socket(TCP client)");

        sockaddr_in dst {};
        dst.sin_family = AF_INET;
        dst.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        dst.sin_port = htons(tcpPort);

        if (net.connect(client, reinterpret_cast<const sockaddr *>(&dst), sizeof(dst)) != 0) {
            fail("connect()", "connect failed");
            // accept thread might be blocked; close server to unblock in most cases
            net.closeSocket(client);
            net.closeSocket(server);
            acceptThread.join();
            return 1;
        }
        ok("connect()");

        socketHandle accepted = acceptedFuture.get();
        acceptThread.join();
        if (accepted == kInvalidSocket) {
            fail("accept()", "invalid socket");
            return 1;
        }
        ok("accept()");

        const char tcpMsg[] = "tcp_smoke";
        auto sTcp = net.send(client, tcpMsg, sizeof(tcpMsg), 0);
        if (sTcp <= 0) {
            fail("send()", "returned <= 0");
            return 1;
        }
        ok("send()");

        char tcpBuf[256] {};
        auto rTcp = net.recv(accepted, tcpBuf, sizeof(tcpBuf), 0);
        if (rTcp <= 0) {
            fail("recv()", "returned <= 0");
            return 1;
        }
        ok("recv()");
        std::cout << "TCP received: " << tcpBuf << "\n";

        net.closeSocket(client);
        net.closeSocket(accepted);
        net.closeSocket(server);
        ok("closeSocket(TCP)");

        if (net.cleanupNetwork() != 0) {
            fail("cleanupNetwork()", "returned non-zero");
            return 1;
        }
        ok("cleanupNetwork()");

        std::cout << "\nAll NetWrapper methods called successfully.\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
}
