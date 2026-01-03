---
id: udp-server
title: Server::UDPServer
sidebar_label: UDP Server
---

`Server::UDPServer` is the concrete implementation of a **UDP-based game server**.

It inherits from `AServer` (which implements the generic server interface `IServer`) and is responsible for:

- creating and configuring a UDP socket,
- binding it to the configured IP/port,
- switching the socket to **non-blocking** mode,
- receiving datagrams and wrapping them into `UDPPacket` objects,
- storing received packets into a ring buffer,
- sending packets to clients.

This page explains the **public interface**, the **lifecycle**, and the **internal behavior** of `UDPServer`.


# Diagram: UDP Flow

```text
┌───────────────────────────────────────────────────────────┐
│                     APPLICATION SERVER                    │
│                                                           │
│   ┌───────────────┐     ┌──────────────────────────────┐  │
│   │  GameServer   │     │          IGameWorld          │  │
│   │ (IMessageSink)│<--->│     (ECS / game engine)      │  │
│   └──────▲────────┘     └──────────────────────────────┘  │
│          │                            ▲                   │
│          │ network→game events        │ game API          │
│   ┌──────┴────────┐                   │                   │
│   │ IMessageSink  │ interface         │                   │
│   └──────▲────────┘                   │                   │
└──────────│────────────────────────────│───────────────────┘
           │                            │
           │                            │
┌──────────┴────────────────────────────┴───────────────────┐
│                       NETWORK SERVER                      │
│                                                           │
│   ┌─────────────────────────────────────────────────────┐ │
│   │                 ServerRuntime                       │ │
│   │  - starts IServer/UdpServer                         │ │
│   │  - thread 1: runReceiver → readPackets()            │ │
│   │  - thread 2: runProcessor → popPacket()             │ │
│   │                        │                            │ │
│   └───────────────▲────────┴────────────────────────────┘ │
│                   │                                       │
│               IPacket                                     │
│                   │                                       │
│   ┌───────────────┴─────────────┐     ┌─────────────────┐ │
│   │        UDPPacketRouter         │     │ SessionManager  │ │
│   │ - reads PacketHeader        │     │ - sockaddr → id │ │
│   │ - validates (size/version)  │     │ - id → sockaddr │ │
│   │ - getOrCreateSession(addr)  │     └─────────────────┘ │
│   │ - parses payload (CONNECT,  │                         │
│   │   INPUT, PING, DISCONNECT)  │                         │
│   │ - calls IMessageSink        │                         │
│   └─────────────────────────────┘                         │
│                                                           │
│   ┌─────────────────────────────────────────────────────┐ │
│   │                     UdpServer (IServer)             │ │
│   │  - socket UDP                                       │ │
│   │  - recvfrom() → IPacket + push RX queue             │ │
│   │  - pop TX queue → sendto()                          │ │
│   └─────────────────────────────────────────────────────┘ │
└───────────────────────────────────────────────────────────┘
```

## 1. Class overview

Header (simplified):

```cpp title="UDPServer.hpp"
namespace Server {

    class UDPServer : public AServer {
      public:
        UDPServer();
        ~UDPServer() override;

        void start() override;
        void stop() override;
        void readPackets() override;

        /// Send one packet to its destination address.
        /// Returns true on success, false on error.
        bool sendPacket(const Net::IPacket &pkt) override;

      private:
        /// Create and configure the UDP socket.
        void setupSocket(const Net::SocketConfig &params,
                         const Net::SocketOptions &optParams);

        /// Bind the socket to the configured IP/port.
        void bindSocket(Net::family_t family = AF_INET);

        /// Ring buffer used to store received packets.
        Buffer::RingBuffer<std::shared_ptr<Net::IPacket>> _rxBuffer;
    };

} // namespace Server
````

Key points:

* `UDPServer` **does not** decide the game logic.
* It is only responsible for **network I/O**:

    * reading raw UDP datagrams,
    * converting them into packet objects,
    * sending packet objects back to clients.

---

## 2. Member fields and invariants

`UDPServer` inherits the following important fields from `AServer` (not shown here, but used):

* `_ip` — server IP as a `std::string`.
* `_port` — server port as `uint16_t`.
* `_socketFd` — socket handle (`socketHandle`).
* `_isRunning` — boolean flag indicating if the server is running.

In addition, `UDPServer` defines:

```cpp
Buffer::RingBuffer<std::shared_ptr<Net::IPacket>> _rxBuffer;
```

* It is created with a **fixed capacity** of 1024 packets in the constructor.
* It stores **shared pointers to `IPacket`** (typically `UDPPacket` instances).
* When the buffer is full, new packets are **dropped** and a warning is printed.

Invariants:

* When no socket is created or after `stop()`, `_socketFd == kInvalidSocket`.
* When the server is running, `_isRunning == true` and `_socketFd` is a valid socket handle.
* `configure(ip, port)` (from `AServer`) must be called before `start()`.

---

## 3. Constructor and destructor

### 3.1. Constructor

```cpp
UDPServer::UDPServer() : AServer(), _rxBuffer(1024)
{
#ifdef _WIN32
    WSADATA wsa;
    const int r = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (r != 0)
        throw ServerError("{UDPServer::UDPServer} WSAStartup failed");
#endif
}
```

Behavior:

* Calls the base `AServer` constructor.
* Initializes the ring buffer with a capacity of **1024 packets**.
* On Windows:

    * calls `WSAStartup` to initialize the Winsock library,
    * throws `ServerError` if this fails.

On non-Windows platforms, the constructor does not perform extra work (other than ring buffer initialization).

### 3.2. Destructor

```cpp
UDPServer::~UDPServer()
{
    if (_isRunning) {
        stop();
    }
#ifdef _WIN32
    WSACleanup();
#endif
}
```

Behavior:

* If the server is still running, it calls `stop()` to shut it down cleanly:

    * this closes the socket and sets `_isRunning = false`.
* On Windows:

    * calls `WSACleanup()` to release Winsock resources.

---

## 4. Starting and stopping the server

### 4.1. `start()`

```cpp
void UDPServer::start()
{
    if (_isRunning || _socketFd != kInvalidSocket)
        throw ServerError("{UDPServer::start} Server is already running");

    try {
        Net::SocketConfig socketParams = {AF_INET, SOCK_DGRAM, IPPROTO_UDP};
        Net::SocketOptions socketOptions = {SOL_SOCKET, SO_REUSEADDR, 1};
        setupSocket(socketParams, socketOptions);
        bindSocket(socketParams.family);
        setNonBlocking(true);
    } catch (const ServerError &e) {
        if (_socketFd != kInvalidSocket)
            Net::NetWrapper::closeSocket(_socketFd);
        _socketFd = kInvalidSocket;
        throw ServerError(std::string("{UDPServer::start}") + e.what());
    }

    _isRunning = true;
    std::cout << "{UDPServer::start} UDP Server started on " << _ip << ":" << _port << std::endl;
}
```

Step-by-step:

1. **Guard check**
   If `_isRunning` is already `true` or `_socketFd` is not `kInvalidSocket`,
   the server is already initialized → throw `ServerError`.

2. **Setup and bind inside a `try`**:

    * Build a `SocketConfig` for UDP:

        * `family = AF_INET` (IPv4),
        * `type = SOCK_DGRAM` (datagrams),
        * `proto = IPPROTO_UDP`.
    * Build a `SocketOptions` struct:

        * `level = SOL_SOCKET`,
        * `optName = SO_REUSEADDR`,
        * `optVal = 1` (enable address reuse).
    * Call `setupSocket(...)` to:

        * validate stored IP and port,
        * create the socket via `NetWrapper::socket`,
        * set `SO_REUSEADDR` via `NetWrapper::setSocketOpt`.
    * Call `bindSocket(...)` to:

        * bind the socket to `_ip` and `_port`.
    * Call `setNonBlocking(true)` (implemented in `AServer`) to make the socket non-blocking.

3. **Error handling**
   If any step throws `ServerError`:

    * if `_socketFd` is valid, close it via `NetWrapper::closeSocket`,
    * reset `_socketFd` to `kInvalidSocket`,
    * re-throw a new `ServerError` with a `{UDPServer::start}` prefix.

4. **Mark running**
   Set `_isRunning = true` and print a log message with IP and port.

Preconditions:

* `configure(ip, port)` must have been called successfully before `start()`.
* IP and port must be valid (checked in `setupSocket()` and `bindSocket()`).

---

### 4.2. `stop()`

```cpp
void UDPServer::stop()
{
    _isRunning = false;
    Net::NetWrapper::closeSocket(_socketFd);
    _socketFd = kInvalidSocket;
    std::cout << "{UDPServer::stop} UDP Server stopped." << std::endl;
}
```

Behavior:

* Marks the server as not running: `_isRunning = false`.
* Closes the underlying socket via `NetWrapper::closeSocket`.
* Resets `_socketFd` to `kInvalidSocket`.
* Logs a message.

After `stop()`:

* The server cannot read or send packets anymore.
* To restart, you must:

    1. call `configure(ip, port)` again if needed,
    2. call `start()`.

---

## 5. Receiving packets: `readPackets()`

```cpp
void UDPServer::readPackets()
{
    std::shared_ptr<Net::IPacket> pkt = std::make_shared<Net::UDPPacket>();
    socklen_t addrLen = sizeof(sockaddr_in);

    recvfrom_return_t received = Net::NetWrapper::recvFrom(
        _socketFd,
        pkt->buffer(),
        Net::UDPPacket::MAX_SIZE,
        0,
        reinterpret_cast<sockaddr *>(const_cast<sockaddr_in *>(pkt->address())),
        &addrLen
    );
    if (received <= 0)
        return;

    pkt->setSize(static_cast<size_t>(received));

    if (!_rxBuffer.push(pkt))
        std::cerr << "{UDPServer::readPackets} Warning: RX buffer overflow, packet dropped\n";

    std::cout << pkt << std::endl;
}
```

Step-by-step:

1. **Allocate a packet**
   Create a `std::shared_ptr<Net::IPacket>` pointing to a new `UDPPacket`.

2. **Prepare address length**
   `addrLen` is initialized to `sizeof(sockaddr_in)`.

3. **Receive from socket**
   Call `NetWrapper::recvFrom` with:

    * the socket handle `_socketFd`,
    * the packet buffer `pkt->buffer()`,
    * the maximum packet size `Net::UDPPacket::MAX_SIZE`,
    * flags = `0`,
    * a pointer to the `sockaddr_in` inside the packet (`pkt->address()`),
    * pointer to `addrLen`.

   `recvFrom` returns the number of bytes read or `<= 0` on error / no data.

4. **Handle “no data” / error**
   If `received <= 0`, simply `return;`:

    * Non-blocking sockets often return immediately when there is no data.
    * Errors may also produce a non-positive value.

5. **Set packet size**
   On success, `setSize(received)` stores the actual size of the datagram inside the packet.

6. **Push into ring buffer**
   Try to push the packet into `_rxBuffer`:

    * If `push(pkt)` returns `false`, the buffer is full.
    * The packet is then **dropped**, and a warning is printed.

7. **Debug print**
   `std::cout << pkt << std::endl;`
   This relies on an overload of `operator<<` for `IPacket` or `UDPPacket`, and is useful for debugging.

Important notes:

* `readPackets()` reads **at most one packet** per call.
* It should be called **regularly** in the main loop:

```cpp
while (server.isRunning()) {
    server.readPackets();
    // Then: pop packets from the internal ring buffer and process them.
}
```

---

## 6. Sending packets: `sendPacket()`

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

Step-by-step:

1. Uses `NetWrapper::sendTo` to send `pkt.size()` bytes from `pkt.buffer()` to the address given by `pkt.address()`.

2. Returns `true` if `sendTo` does **not** return `-1`, meaning:

    * the OS accepted the datagram for delivery.

3. Returns `false` if there was an error (e.g. invalid socket, invalid address, network error).

Important constraints:

* The packet **must** contain a valid destination address (`sockaddr_in`) set via `pkt.setAddress(...)` somewhere before calling `sendPacket`.
* The packet size (`pkt.size()`) should not exceed `UDPPacket::MAX_SIZE`.

Typical usage:

```cpp
Net::UDPPacket pkt;
// Fill payload into pkt.buffer(), set pkt.setSize(...)
// Set destination address pkt.setAddress(clientAddr)

server.sendPacket(pkt);
```

The higher-level code (game logic / protocol layer) is responsible for:

* encoding application data into the packet buffer,
* using the correct endianness helpers (`Endian.hpp`),
* setting the correct address and size.

---

## 7. Internal helpers: `setupSocket` and `bindSocket`

### 7.1. `setupSocket(...)`

```cpp
void UDPServer::setupSocket(const Net::SocketConfig &params,
                            const Net::SocketOptions &optParams)
{
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::setupSocket} Invalid IP address or port number");

    socketHandle sockFd =
        Net::NetWrapper::socket(static_cast<int>(params.family), params.type, params.proto);
    if (sockFd == kInvalidSocket)
        throw ServerError("{UDPServer::setupSocket} Failed to create socket");

    int opt = optParams.optVal;
    if (Net::NetWrapper::setSocketOpt(
            sockFd,
            optParams.level,
            optParams.optName,
            reinterpret_cast<const char *>(&opt),
            sizeof(opt)
        ) < 0) {
        Net::NetWrapper::closeSocket(sockFd);
        throw ServerError("{UDPServer::setupSocket} Failed to set socket options");
    }

    _socketFd = sockFd;
}
```

Responsibilities:

* Validate the stored IP and port (`isStoredIpCorrect()`, `isStoredPortCorrect()` from `AServer`).
* Create the socket via `NetWrapper::socket(family, type, proto)`.
* Apply socket options (usually `SO_REUSEADDR`) via `NetWrapper::setSocketOpt`.
* On error:

    * close the temporary `sockFd`,
    * throw a `ServerError`.
* On success:

    * assign `_socketFd = sockFd`.

### 7.2. `bindSocket(...)`

```cpp
void UDPServer::bindSocket(Net::family_t family)
{
    if (_socketFd == kInvalidSocket)
        throw ServerError("{UDPServer::bindSocket} Socket not initialized");
    if (!isStoredIpCorrect() || !isStoredPortCorrect())
        throw ServerError("{UDPServer::bindSocket} Invalid IP address or port number");

    sockaddr_in addr = {};
    addr.sin_family = family;
    addr.sin_port = htons(_port);
    if (inet_pton(family, _ip.c_str(), &addr.sin_addr) <= 0)
        throw ServerError("{UDPServer::bindSocket} Invalid IP address format");

    int result = bind(_socketFd,
                      reinterpret_cast<struct sockaddr *>(&addr),
                      sizeof(addr));
    if (result != 0)
        throw ServerError("{UDPServer::bindSocket} Failed to bind socket");
}
```

Responsibilities:

* Ensure `_socketFd` is valid (not `kInvalidSocket`).
* Re-validate IP and port.
* Fill a `sockaddr_in` with:

    * `sin_family = family`,
    * `sin_port = htons(_port)`,
    * `sin_addr` from `_ip` via `inet_pton`.
* Call `bind()` to bind the socket to the IP/port.
* Throw `ServerError` if:

    * the IP format is invalid,
    * `bind()` fails.

---

## 8. Typical usage pattern

Example of how `UDPServer` fits into a simple main loop:

```cpp
#include "UDPServer.hpp"

int main()
{
    Server::UDPServer server;

    try {
        // 1. Configure server IP and port
        server.configure("127.0.0.1", 4242);

        // 2. Start the UDP server (create + bind socket, non-blocking)
        server.start();
    } catch (const Server::ServerError &e) {
        std::cerr << "Failed to start UDP server: " << e.what() << std::endl;
        return 1;
    }

    while (server.isRunning()) {
        // 3. Poll network
        server.readPackets();

        // 4. Process packets from the internal ring buffer
        //    (this part is implemented elsewhere, not in UDPServer)

        // 5. Build and send response packets
        //    server.sendPacket(...);

        // 6. Decide when to stop (signal handler, command, etc.)
        // if (shouldQuit()) {
        //     server.stop();
        // }
    }

    return 0;
}
```

In a real project, the main loop also:

* integrates a **signal handler** (for clean shutdown),
* connects the network packets to the **ECS**,
* performs **game updates** and **state replication** to clients.

`UDPServer` focuses only on the **network transport**, not on the game rules.