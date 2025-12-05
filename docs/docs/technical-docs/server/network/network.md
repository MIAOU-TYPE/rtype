---
id: overview
title: Overview
sidebar_label: Overview
---

The network layer is responsible for **sending and receiving UDP datagrams** and
exposing them to the rest of the server as **C++ objects**, not raw bytes.

This page explains:

- what the network layer does,
- which main classes are involved,
- how data flows from the client to the game logic and back.

It is the **entry point** for understanding everything related to networking in this project.

---

## 1. Goals of the network layer

The network layer has four main goals:

1. **Create and configure a UDP socket** (IP, port, options…)
2. **Receive datagrams from clients** and wrap them in C++ objects
3. **Send datagrams to clients** in a consistent format (endianness, payload…)
4. **Stay portable** across platforms (Linux, macOS, Windows)

The rest of the code (ECS, gameplay, etc.) should not care about:

- how `socket()`, `recvfrom()` or `sendto()` work,
- what type the socket handle is (`int`, `SOCKET`, …),
- OS-specific headers or system calls.

All of this is hidden behind the network layer.

---

## 2. Main building blocks

At a high level, the network layer is composed of:

- **Server abstraction**
  - `IServer` / `AServer`: common server interface and base implementation
  - `UDPServer`: concrete UDP server implementation

- **Low-level socket wrapper**
  - `NetWrapper`: thin wrapper around OS networking functions
  - `SocketConfig` / `SocketOptions`: how the socket is configured

- **Packet abstraction**
  - `IPacket`: generic interface for “a packet received or sent by the server”
  - `UDPPacket`: concrete implementation for UDP datagrams

- **Buffering**
  - `RingBuffer<std::shared_ptr<IPacket>>`: queue of received packets

- **Endianness helpers**
  - `Endian.hpp`: functions to convert integers / floats to a consistent network format

You will find these parts in folders similar to:

- `server/src/server/…` (server + UDP server)
- `server/src/server/NetWrapper/…` (socket wrapper)
- `server/src/packet/…` (packet abstractions)
- `server/src/buffer/…` (ring buffer)
- `server/src/network/Endian.hpp` (endianness helpers)

---

## 3. Data flow: from client to game logic

### 3.1. Incoming data

The path of an **incoming UDP datagram** looks like this:

1. A client sends a UDP datagram to the server IP/port.
2. The operating system receives it on the **UDP socket**.
3. `NetWrapper::recvFrom()` reads the datagram from the socket.
4. The server wraps the raw bytes into a **`UDPPacket`** object.
5. The `UDPPacket` is pushed into a **`RingBuffer<IPacket>`**.
6. The game logic (ECS, systems, etc.) later consumes packets from this buffer.

In diagram form:

```text
Client (UDP)  -->  OS Socket  -->  NetWrapper::recvFrom()
   -->  UDPPacket (implements IPacket)
   -->  RingBuffer<IPacket>
   -->  Game logic / ECS
````

### 3.2. Outgoing data

For **outgoing** data, the flow is reversed:

1. The game logic decides to send something to a client (for example “spawn entity”, “update position”…).
2. It builds a packet object (usually implementing `IPacket`, e.g. `UDPPacket` or a derived type).
3. It sets the destination address (client IP + port) inside the packet.
4. It calls `UDPServer::sendPacket(packet)`.
5. `UDPServer` calls `NetWrapper::sendTo()` with the packet buffer.
6. The OS sends the datagram to the client.

```text
Game logic / ECS
   -->  Packet object (IPacket / UDPPacket)
   -->  UDPServer::sendPacket()
   -->  NetWrapper::sendTo()
   -->  OS Socket  -->  Client (UDP)
```

---

## 4. Role of each main class

### 4.1. `IServer` and `AServer`

These classes define the **generic server API**.

Typical responsibilities:

* store the server IP and port,
* expose methods like:

    * `configure(ip, port)`
    * `start()`
    * `stop()`
    * `readPackets()` (to poll the network)
    * `sendPacket(const IPacket&)`
    * `isRunning()`

`IServer` is usually a pure interface, and `AServer` is a **base implementation** that:

* validates IP and port,
* manages the socket handle (`_socketFd`),
* provides utility methods (e.g. `setNonBlocking`).

`UDPServer` inherits from `AServer` and implements the UDP-specific parts.

---

### 4.2. `UDPServer`

`UDPServer` is the **concrete UDP implementation** of the server.

Main responsibilities:

* create and configure a UDP socket,
* bind it to the configured IP/port,
* switch it to **non-blocking** mode,
* read datagrams from the socket (`readPackets()`),
* send datagrams (`sendPacket()`),
* push incoming packets into a `RingBuffer`.

`UDPServer` is the main entry point used by the rest of the application.
From the outside, you only need to:

```cpp
UDPServer server;
server.configure("127.0.0.1", 4242);
server.start();

while (server.isRunning()) {
    server.readPackets();
    // Consume packets from the ring buffer
    // Update ECS, build response packets, call server.sendPacket(...)
}
```

The low-level calls (`recvfrom`, `sendto`, `bind`, `setsockopt`, etc.) are hidden.

---

### 4.3. `NetWrapper`, `SocketConfig`, `SocketOptions`

`NetWrapper` is a **thin wrapper around OS networking APIs**.

Its job is to hide platform differences between Windows and Unix:

* type of socket handle (`SOCKET` vs `int`),
* which headers to include,
* which function to call for `close`, etc.

Typical static methods:

* `socket(domain, type, protocol)`
* `closeSocket(handle)`
* `setSocketOpt(handle, level, optName, optValue, optLen)`
* `recvFrom(handle, buffer, len, flags, srcAddr, addrLen)`
* `sendTo(handle, buffer, len, flags, destAddr, addrLen)`

`SocketConfig` and `SocketOptions` are small structs that describe:

* **what kind of socket** we want (IPv4/IPv6, UDP/TCP, protocol…),
* **what options** we apply (reuse address, timeouts, etc.).

This makes `UDPServer` code more readable and avoids repeating magic constants.

---

### 4.4. `IPacket` and `UDPPacket`

`IPacket` is an **abstract representation of a packet** inside the server.
It provides methods like:

* `uint8_t* buffer()` / `const uint8_t* buffer() const`
* `size_t size() const` / `void setSize(size_t)`
* `const sockaddr_in* address() const` / `void setAddress(const sockaddr_in&)`
* `size_t capacity() const` (max buffer size)

`UDPPacket` is the **concrete implementation** used for UDP:

* internally stores:

    * a fixed-size byte buffer (e.g. 2048 bytes),
    * the current size of the used part,
    * the client address (IP + port).
* when `UDPServer::readPackets()` receives data:

    * it fills a new `UDPPacket`,
    * sets its size,
    * stores the source address,
    * pushes it into the ring buffer.

From the rest of the application, you only deal with `IPacket` pointers or references,
not with raw `uint8_t*` buffers.

---

### 4.5. `RingBuffer<std::shared_ptr<IPacket>>`

The ring buffer is a **fixed-size queue** of packets.

Why?

* The server should not block on `recvfrom()` waiting for clients.
* At the same time, the game logic may take some time to process packets.
* A ring buffer lets the network layer **push packets quickly**, and the game logic
  **pull them later** at its own pace.

Typical behavior:

* `push()` adds a packet if there is free space.
* If the buffer is full, the packet is dropped (and a warning is usually logged).
* `pop()` returns the next packet to process.

This decoupling is very important in a real-time game server, where peaks in network traffic
should not freeze the whole application.

---

### 4.6. `Endian.hpp`

Different CPU architectures use different **endianness**:

* little-endian (most PCs),
* big-endian (some CPUs, some network hardware, etc.).

To avoid inconsistencies, the network layer must define **one canonical format** for data
sent over the network (traditionally big-endian / “network byte order”).

`Endian.hpp` provides helper functions to:

* convert integers from host order to network order and back,
* safely convert floats by reinterpreting them as `uint32_t` and swapping bytes.

When you build packet payloads, you should **always** use these helpers so that:

* the server and the client agree on how data is encoded,
* the protocol remains stable across platforms.

---

## 5. Interaction with ECS and game logic

The network layer does **not** know anything about:

* entities,
* components,
* systems,
* gameplay rules.

Instead, it only knows how to:

* read bytes from the socket,
* package them into `IPacket` objects,
* send `IPacket` objects to clients.

The **glue** between networking and game logic is typically:

1. A piece of code that:

    * pops packets from the ring buffer,
    * decodes packet contents (IDs, payload, etc.),
    * applies changes to the ECS (move entity, create player, apply damage…).

2. Another piece of code that:

    * reads the ECS state,
    * decides what packets to send (e.g. position updates),
    * builds `IPacket` objects,
    * calls `server.sendPacket()`.

This separation keeps the codebase maintainable:
you can modify the ECS or the network layer **independently** as long as the
packet format and high-level interface stay consistent.

---

## 6. Summary

* The **network layer** is a clean abstraction around UDP networking.
* `UDPServer` is the main entry point: it uses `NetWrapper` and `UDPPacket` internally.
* `NetWrapper` hides OS details and exposes simple static functions.
* `IPacket` / `UDPPacket` wrap raw bytes into C++ objects with an address.
* `RingBuffer` decouples the speed of the network from the speed of the game logic.
* `Endian.hpp` guarantees that values are encoded in a consistent format on the wire.