---
id: netwrapper-and-sockets
title: NetWrapper & Socket Configuration
sidebar_label: NetWrapper & sockets
---

The `NetWrapper` module is a **thin abstraction layer** around OS networking APIs
(`socket`, `recvfrom`, `sendto`, `setsockopt`, `close`, etc.).

Its goals are:

- to **hide platform-specific details** (Windows vs Linux/macOS),
- to provide a **clean, consistent C++ API** for the rest of the code,
- to centralize error-prone low-level calls in one place.

`UDPServer` uses `NetWrapper` to create, configure, read from and write to the
underlying UDP socket.

This page explains:

- how socket types are unified across platforms,
- what functions `NetWrapper` exposes,
- how `SocketConfig` and `SocketOptions` are used,
- how this all connects to `UDPServer`.

---

## 1. Cross-platform socket types

In the header, `NetWrapper` defines platform-dependent types:

```cpp title="NetWrapper.hpp (simplified)"
namespace Net {

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>

    using socketHandle = SOCKET;
    constexpr socketHandle kInvalidSocket = INVALID_SOCKET;

    using recvfrom_return_t = int;
    using sendto_return_t   = int;

#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>

    using socketHandle = int;
    constexpr socketHandle kInvalidSocket = -1;

    using recvfrom_return_t = ssize_t;
    using sendto_return_t   = ssize_t;
#endif

} // namespace Net
````

Key points:

* On **Windows**:

    * `socketHandle` is an alias for `SOCKET`.
    * An invalid socket is `INVALID_SOCKET`.
    * `recvfrom` / `sendto` return `int`.

* On **Unix-like systems (Linux, macOS, …)**:

    * `socketHandle` is an alias for `int`.
    * An invalid socket is `-1`.
    * `recvfrom` / `sendto` return `ssize_t`.

For the rest of the code (e.g. `UDPServer`), we only use:

* `Net::socketHandle`
* `Net::kInvalidSocket`
* `Net::recvfrom_return_t`
* `Net::sendto_return_t`

and we never directly care about the platform-specific type.

---

## 2. NetWrapper API

The `NetWrapper` class typically exposes a set of **static methods** that wrap
system calls:

```cpp title="NetWrapper.hpp (simplified)"
namespace Net {

    class NetWrapper {
      public:
        static socketHandle socket(int domain, int type, int protocol);
        static void closeSocket(socketHandle s);

        static int setSocketOpt(socketHandle s,
                                int level,
                                int optName,
                                const void *optVal,
                                int optLen);

        static recvfrom_return_t recvFrom(socketHandle sockFd,
                                          void *buf,
                                          size_t len,
                                          int flags,
                                          struct sockaddr *srcAddr,
                                          socklen_t *addrLen);

        static sendto_return_t sendTo(socketHandle sockFd,
                                      const void *buf,
                                      size_t len,
                                      int flags,
                                      const struct sockaddr *destAddr,
                                      socklen_t addrLen);
    };

} // namespace Net
```

The implementation of each method contains the **real** system calls (`::socket`,
`::setsockopt`, `::recvfrom`, `::sendto`, `::closesocket` / `::close`, etc.),
but the rest of the code does not need to know this.

---

## 3. Creating a socket: `socket()`

```cpp
socketHandle NetWrapper::socket(int domain, int type, int protocol);
```

* Wraps `::socket(domain, type, protocol)` from the OS.
* Returns:

    * a **valid** `socketHandle` on success,
    * `kInvalidSocket` on failure.

Typical usage in `UDPServer::setupSocket()`:

```cpp
Net::SocketConfig socketParams = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
socketHandle sockFd = Net::NetWrapper::socket(
    static_cast<int>(socketParams.family),
    socketParams.type,
    socketParams.proto
);
if (sockFd == kInvalidSocket) {
    throw ServerError("{UDPServer::setupSocket} Failed to create socket");
}
```

With this approach:

* `UDPServer` knows it wants an IPv4 (`AF_INET`), UDP (`SOCK_DGRAM`) socket.
* It does **not** care if the underlying type is `SOCKET` or `int`.

---

## 4. Closing a socket: `closeSocket()`

```cpp
void NetWrapper::closeSocket(socketHandle s);
```

* Wraps `::closesocket(s)` on Windows.
* Wraps `::close(s)` on Unix systems.
* Used in:

    * `UDPServer::stop()`,
    * error paths in `UDPServer::start()` / `setupSocket()`.

Example:

```cpp
void UDPServer::stop()
{
    _isRunning = false;
    Net::NetWrapper::closeSocket(_socketFd);
    _socketFd = kInvalidSocket;
}
```

This ensures:

* the OS resources for the socket are released,
* the internal handle is reset to an invalid value.

---

## 5. Socket options: `setSocketOpt()`

```cpp
int NetWrapper::setSocketOpt(socketHandle s,
                             int level,
                             int optName,
                             const void *optVal,
                             int optLen);
```

* Wraps `::setsockopt(s, level, optName, optVal, optLen)`.
* Returns `< 0` on error, `>= 0` on success (same convention as the OS function).

In `UDPServer::setupSocket()`, it is typically used to enable `SO_REUSEADDR`:

```cpp
Net::SocketOptions optParams = { SOL_SOCKET, SO_REUSEADDR, 1 };

int opt = optParams.optVal;
if (Net::NetWrapper::setSocketOpt(
        sockFd,
        optParams.level,
        optParams.optName,
        reinterpret_cast<const char*>(&opt),
        sizeof(opt)
    ) < 0) {
    Net::NetWrapper::closeSocket(sockFd);
    throw ServerError("{UDPServer::setupSocket} Failed to set socket options");
}
```

Why `SO_REUSEADDR`?

* It allows the address/port to be reused more quickly after the server restarts.
* Without it, the OS can keep the port in `TIME_WAIT` state, and the bind might fail.

---

## 6. Receiving data: `recvFrom()`

```cpp
recvfrom_return_t NetWrapper::recvFrom(socketHandle sockFd,
                                       void *buf,
                                       size_t len,
                                       int flags,
                                       struct sockaddr *srcAddr,
                                       socklen_t *addrLen);
```

* Wraps `::recvfrom(sockFd, buf, len, flags, srcAddr, addrLen)`.
* Reads up to `len` bytes from `sockFd` into `buf`.
* Fills `*srcAddr` with the sender's address.
* Updates `*addrLen` with the size of the address.
* Returns:

    * number of bytes received on success,
    * `0` or negative value on error or when there is no data (for non-blocking sockets).

Usage in `UDPServer::readPackets()`:

```cpp
std::shared_ptr<Net::IPacket> pkt = std::make_shared<Net::UDPPacket>();
socklen_t addrLen = sizeof(sockaddr_in);

recvfrom_return_t received = Net::NetWrapper::recvFrom(
    _socketFd,
    pkt->buffer(),
    Net::UDPPacket::MAX_SIZE,
    0,
    reinterpret_cast<sockaddr*>(const_cast<sockaddr_in*>(pkt->address())),
    &addrLen
);
if (received <= 0)
    return;

pkt->setSize(static_cast<size_t>(received));
_rxBuffer.push(pkt);
```

Key points:

* `UDPServer::readPackets()` only processes one packet per call.
* On **non-blocking** sockets:

    * If there is no data, `recvFrom()` returns immediately with a non-positive value.
    * `readPackets()` simply returns without doing anything.
* On success:

    * the packet buffer is filled with `received` bytes,
    * the sender address is stored inside the packet,
    * the packet is queued in the ring buffer.

---

## 7. Sending data: `sendTo()`

```cpp
sendto_return_t NetWrapper::sendTo(socketHandle sockFd,
                                   const void *buf,
                                   size_t len,
                                   int flags,
                                   const struct sockaddr *destAddr,
                                   socklen_t addrLen);
```

* Wraps `::sendto(sockFd, buf, len, flags, destAddr, addrLen)`.
* Sends `len` bytes from `buf` to the address `destAddr`.
* Returns:

    * the number of bytes sent on success,
    * `-1` on error.

Usage in `UDPServer::sendPacket()`:

```cpp
bool UDPServer::sendPacket(const Net::IPacket &pkt)
{
    return Net::NetWrapper::sendTo(
               _socketFd,
               pkt.buffer(),
               pkt.size(),
               0,
               reinterpret_cast<const sockaddr *>(pkt.address()),
               sizeof(sockaddr_in)
           ) != -1;
}
```

Important details:

* The packet must have:

    * a valid destination address set via `pkt.setAddress(...)`,
    * a valid size set via `pkt.setSize(...)`.
* `sendPacket()` simply checks if `sendTo` returned `-1` or not, and returns `true`/`false`.

The **higher-level protocol code** is responsible for:

* filling the packet buffer with encoded data,
* choosing the correct destination address (client IP + port),
* calling `server.sendPacket(pkt)`.

---

## 8. SocketConfig & SocketOptions

To avoid scattering magic constants across the code, the project uses two small structs:

```cpp title="SocketConfig & SocketOptions (simplified)"
namespace Net {

    using family_t = sa_family_t;

    struct SocketConfig {
        family_t family = AF_INET;   // IPv4
        int type       = SOCK_DGRAM; // UDP datagrams
        int proto      = IPPROTO_UDP;
    };

    struct SocketOptions {
        int level  = SOL_SOCKET;
        int optName = SO_REUSEADDR;
        int optVal  = 1;             // enabled
    };

} // namespace Net
```

They are passed to `UDPServer::setupSocket()`:

```cpp
void UDPServer::start()
{
    Net::SocketConfig socketParams = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
    Net::SocketOptions socketOptions = {SOL_SOCKET, SO_REUSEADDR, 1};

    setupSocket(socketParams, socketOptions);
    bindSocket(socketParams.family);
    setNonBlocking(true);
}
```

Benefits:

* Makes the code **self-documenting** (you see clearly what type of socket is created).
* Allows future changes (e.g. IPv6, different options) in **one place**.

---

## 9. Relation to UDPServer

`UDPServer` uses `NetWrapper` and these structs in its internal methods:

* `setupSocket(SocketConfig, SocketOptions)`:

    * validates stored IP and port,
    * calls `NetWrapper::socket(...)`,
    * calls `NetWrapper::setSocketOpt(...)`,
    * assigns `_socketFd`.

* `bindSocket(family)`:

    * constructs a `sockaddr_in` with `_ip` and `_port`,
    * calls `::bind(...)` directly (or via a wrapper, depending on implementation),
    * throws `ServerError` on failure.

* `readPackets()`:

    * calls `NetWrapper::recvFrom(...)` into a `UDPPacket` buffer.

* `sendPacket()`:

    * calls `NetWrapper::sendTo(...)` with a packet buffer and destination address.

In other words:

> `UDPServer` decides **what** it wants to do (UDP server, non-blocking, with a ring buffer).
> `NetWrapper` decides **how** to talk to the OS in a cross-platform way.

---

## 10. Manual example (for understanding)

The following example shows how you could use `NetWrapper` **without** `UDPServer`
(just to understand what `UDPServer` is doing internally):

```cpp
#include "NetWrapper.hpp"
#include "socketParams.hpp"   // SocketConfig, SocketOptions

int main()
{
    // 1. Configure the socket type and options
    Net::SocketConfig cfg   = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
    Net::SocketOptions opt  = {SOL_SOCKET, SO_REUSEADDR, 1};

    // 2. Create socket
    Net::socketHandle sock = Net::NetWrapper::socket(cfg.family, cfg.type, cfg.proto);
    if (sock == Net::kInvalidSocket) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // 3. Apply options
    int value = opt.optVal;
    if (Net::NetWrapper::setSocketOpt(
            sock,
            opt.level,
            opt.optName,
            &value,
            sizeof(value)
        ) < 0) {
        Net::NetWrapper::closeSocket(sock);
        std::cerr << "Failed to set socket options\n";
        return 1;
    }

    // 4. Bind manually (same as UDPServer::bindSocket)
    sockaddr_in addr{};
    addr.sin_family = cfg.family;
    addr.sin_port   = htons(4242);
    inet_pton(cfg.family, "127.0.0.1", &addr.sin_addr);

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        Net::NetWrapper::closeSocket(sock);
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    // 5. Receive once
    char buffer[2048];
    sockaddr_in src{};
    socklen_t srcLen = sizeof(src);

    Net::recvfrom_return_t n = Net::NetWrapper::recvFrom(
        sock,
        buffer,
        sizeof(buffer),
        0,
        reinterpret_cast<sockaddr*>(&src),
        &srcLen
    );

    if (n > 0) {
        std::cout << "Received " << n << " bytes\n";
    }

    // 6. Clean up
    Net::NetWrapper::closeSocket(sock);
    return 0;
}
```

You do **not** need to write this code in your project, because `UDPServer`
already does it for you, but it helps to understand how `NetWrapper` is used.

---

## 11. Summary

* `NetWrapper` wraps **low-level OS networking calls** in a cross-platform way.
* `socketHandle` and `kInvalidSocket` abstract away the differences between
  Windows and Unix.
* `SocketConfig` and `SocketOptions` describe what kind of socket we want and
  which options we apply.
* `UDPServer` builds on top of this to:

    * create, configure and bind a UDP socket,
    * receive datagrams with `recvFrom`,
    * send datagrams with `sendTo`.
