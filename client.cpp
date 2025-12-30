// tcp_client_smoke.cpp
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

static void writeU32BE(uint8_t *p, uint32_t v)
{
    p[0] = static_cast<uint8_t>((v >> 24) & 0xFF);
    p[1] = static_cast<uint8_t>((v >> 16) & 0xFF);
    p[2] = static_cast<uint8_t>((v >> 8) & 0xFF);
    p[3] = static_cast<uint8_t>(v & 0xFF);
}

static uint32_t readU32BE(const uint8_t *p)
{
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}

static bool sendAll(int s, const uint8_t *data, size_t len)
{
    size_t off = 0;
    while (off < len) {
#ifdef _WIN32
        int r = ::send(s, reinterpret_cast<const char *>(data + off), static_cast<int>(len - off), 0);
#else
        ssize_t r = ::send(s, data + off, len - off, 0);
#endif
        if (r <= 0)
            return false;
        off += static_cast<size_t>(r);
    }
    return true;
}

static bool recvAll(int s, uint8_t *data, size_t len)
{
    size_t off = 0;
    while (off < len) {
#ifdef _WIN32
        int r = ::recv(s, reinterpret_cast<char *>(data + off), static_cast<int>(len - off), 0);
#else
        ssize_t r = ::recv(s, data + off, len - off, 0);
#endif
        if (r <= 0)
            return false;
        off += static_cast<size_t>(r);
    }
    return true;
}

int main(int ac, char **av)
{
    const char *ip = (ac >= 2) ? av[1] : "127.0.0.1";
    int port = (ac >= 3) ? std::stoi(av[2]) : 4242;
    std::string msg = (ac >= 4) ? av[3] : "hello_from_client";

#ifdef _WIN32
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 84;
    }
#endif

    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        std::cerr << "socket failed\n";
        return 84;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(port));
#ifdef _WIN32
    if (InetPtonA(AF_INET, ip, &addr.sin_addr) != 1) {
        std::cerr << "Invalid IP\n";
        return 84;
    }
#else
    if (::inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        std::cerr << "Invalid IP\n";
        return 84;
    }
#endif

    if (::connect(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0) {
        std::cerr << "connect failed\n";
        return 84;
    }

    // SEND frame
    std::vector<uint8_t> frame(4 + msg.size());
    writeU32BE(frame.data(), static_cast<uint32_t>(msg.size()));
    std::memcpy(frame.data() + 4, msg.data(), msg.size());

    if (!sendAll(s, frame.data(), frame.size())) {
        std::cerr << "sendAll failed\n";
        return 84;
    }

    std::cout << "[client] sent: " << msg << "\n";

    // READ response frame
    uint8_t hdr[4];
    if (!recvAll(s, hdr, 4)) {
        std::cerr << "recv header failed\n";
        return 84;
    }
    uint32_t len = readU32BE(hdr);
    if (len == 0 || len > 64 * 1024) {
        std::cerr << "bad len: " << len << "\n";
        return 84;
    }

    std::vector<uint8_t> payload(len);
    if (!recvAll(s, payload.data(), payload.size())) {
        std::cerr << "recv payload failed\n";
        return 84;
    }

    std::cout << "[client] received: " << std::string(reinterpret_cast<char *>(payload.data()), payload.size()) << "\n";

#ifdef _WIN32
    closesocket(s);
    WSACleanup();
#else
    ::close(s);
#endif
    return 0;
}
