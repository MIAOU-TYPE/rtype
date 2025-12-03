# Networking

## Overview

The R-Type client network module manages all UDP communication between the client and the game server. It is built with a modular architecture that cleanly separates protocol, transport, and platform-specific wrappers.

---

## Architecture

### 📁 Folder Structure

```
client/src/networks/
├── interfaces/          # Abstract interfaces
│   └── IClientPacket.hpp
├── protocol/            # Protocol definition
│   ├── PacketBase.hpp
│   ├── PacketHeader.hpp
│   ├── client/          # Packets sent by the client
│   │   ├── PacketConnect.hpp
│   │   └── PacketInput.hpp
│   └── server/          # Packets received from the server
│       ├── PacketCreate.hpp
│       ├── PacketDamage.hpp
│       ├── PacketDestroy.hpp
│       └── PacketSnapshot.hpp
├── udp/                 # UDP implementation
│   ├── PacketTypes.hpp
│   ├── UDPClientPacket.hpp
│   └── UDPClientPacket.cpp
└── wrappers/            # Cross-platform abstraction
    ├── NetWrapper.hpp
    └── NetWrapper.cpp
```

---

## Main Components

### 1. **IClientPacket Interface**

📄 *File:* `interfaces/IClientPacket.hpp`

Abstract interface defining the contract for all client packets.

#### Methods

| Method            | Description                        | Return Type          |
| ----------------- | ---------------------------------- | -------------------- |
| `buffer()`        | Retrieves the packet's data buffer | `uint8_t*`           |
| `buffer() const`  | Const version of `buffer()`        | `const uint8_t*`     |
| `size()`          | Returns packet size in bytes       | `size_t`             |
| `setSize(size_t)` | Defines the packet size            | `void`               |
| `address()`       | Returns packet source address      | `sockaddr_in*`       |
| `address() const` | Const version                      | `const sockaddr_in*` |

#### Exception: `PacketError`

Used to report packet-related errors.

---

### 2. **UDPClientPacket**

📄 *Files:* `udp/UDPClientPacket.hpp`, `udp/UDPClientPacket.cpp`

Concrete implementation of `IClientPacket` for UDP communication.

#### Features

* Maximum size: **2048 bytes (MAX_SIZE)**
* Internal dynamic buffer
* Integrated `sockaddr_in` source address handling

**Example:**

```cpp
Network::UDPClientPacket packet;
packet.setSize(100);
uint8_t* data = packet.buffer();
```

---

### 3. **NetWrapper**

📄 *Files:* `wrappers/NetWrapper.hpp`, `NetWrapper.cpp`

Cross-platform abstraction handling low-level socket operations on both Windows (Winsock) and Linux.

#### Static Methods

| Method                           | Description               |
| -------------------------------- | ------------------------- |
| `socket(domain, type, protocol)` | Creates a socket          |
| `closeSocket(handle)`            | Closes a socket           |
| `setSocketOpt(...)`              | Configures socket options |
| `recvFrom(...)`                  | Receives data             |
| `sendTo(...)`                    | Sends data                |

#### Error Handling

Errors throw `NetWrapperError`.

```cpp
try {
    socketHandle s = Network::NetWrapper::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
} catch (const Network::NetWrapperError& e) {
    std::cerr << e.what() << std::endl;
}
```

---

## Communication Protocol

### PacketHeader

📄 *File:* `protocol/PacketHeader.hpp`

Each packet begins with a 4-byte header:

| Field     | Type       | Size | Description             |
| --------- | ---------- | ---- | ----------------------- |
| `type`    | `uint8_t`  | 1    | Packet type             |
| `version` | `uint8_t`  | 1    | Protocol version        |
| `size`    | `uint16_t` | 2    | Total size (big endian) |

Uses `#pragma pack(push, 1)` to enforce identical memory layout.

---

### Packet Types

📄 *File:* `udp/PacketTypes.hpp`

#### Client → Server

| Name         | Value | Description        |
| ------------ | ----- | ------------------ |
| `CONNECT`    | 0x01  | Connection request |
| `DISCONNECT` | 0x02  | Disconnect         |
| `INPUT`      | 0x03  | Player input       |
| `PING`       | 0x04  | Latency test       |

#### Server → Client

| Name             | Value | Description         |
| ---------------- | ----- | ------------------- |
| `ACCEPT`         | 0x10  | Connection accepted |
| `REJECT`         | 0x11  | Connection refused  |
| `SNAPSHOT`       | 0x12  | Game state          |
| `ENTITY_CREATE`  | 0x13  | Entity created      |
| `ENTITY_DESTROY` | 0x14  | Entity destroyed    |
| `PONG`           | 0x15  | Ping response       |
| `DAMAGE_EVENT`   | 0x16  | Damage taken        |
| `GAME_OVER`      | 0x17  | Game over           |

---

## Packet Structures

### Client Packets

#### PacketBase

```cpp
struct PacketBase {
    PacketHeader header;
};
```

#### PacketConnect

| Field      | Type         | Description      |
| ---------- | ------------ | ---------------- |
| `header`   | PacketHeader | Common header    |
| `clientId` | `uint32_t`   | Unique client ID |

**Total size: 8 bytes**

#### PacketInput

| Field      | Type         | Description       |
| ---------- | ------------ | ----------------- |
| `header`   | PacketHeader | Common header     |
| `entity`   | `uint32_t`   | Entity controlled |
| `dx`       | `float`      | Horizontal axis   |
| `dy`       | `float`      | Vertical axis     |
| `shooting` | `uint8_t`    | Shooting flag     |

**Total size: 17 bytes**

---

### Server Packets

#### SnapshotEntity

| Field    | Type       | Description |
| -------- | ---------- | ----------- |
| `id`     | `uint32_t` | Entity ID   |
| `x`      | `float`    | X position  |
| `y`      | `float`    | Y position  |
| `vx`     | `float`    | Velocity X  |
| `vy`     | `float`    | Velocity Y  |
| `sprite` | `uint16_t` | Sprite ID   |

#### PacketSnapshot

| Field         | Type             | Description        |
| ------------- | ---------------- | ------------------ |
| `header`      | PacketHeader     | Common header      |
| `tick`        | `uint32_t`       | Server tick ID     |
| `entityCount` | `uint16_t`       | Number of entities |
| `entities[]`  | SnapshotEntity[] | Entity list        |

**Size: `10 + (22 × entityCount)`**

#### PacketEntityCreate

**Size: 18 bytes**

#### PacketEntityDestroy

**Size: 8 bytes**

#### PacketDamage

**Size: 10 bytes**

---

## Endianness

All multi-byte numeric values use **network byte order (big endian)**.

```cpp
// Host → Network
uint16_t n16 = htons(h16);
uint32_t n32 = htonl(h32);

// Network → Host
uint16_t h16 = ntohs(n16);
uint32_t h32 = ntohl(n32);
```

---

## Flow Diagram

```
Client                          Server
  |                               |
  |------- CONNECT (0x01) ------->|
  |                               |
  |<------ ACCEPT (0x10) ---------|
  |                               |
  |------- INPUT (0x03) --------->|
  |------- INPUT (0x03) --------->|
  |                               |
  |<----- SNAPSHOT (0x12) --------|
  |<-- ENTITY_CREATE (0x13) ------|
  |                               |
  |------- PING (0x04) ---------->|
  |<------ PONG (0x15) -----------|
  |                               |
  |<---- DAMAGE_EVENT (0x16) -----|
  |<-- ENTITY_DESTROY (0x14) -----|
  |                               |
  |----- DISCONNECT (0x02) ------>|
```

---

## Best Practices

* Validate packet sizes
* Handle endianness conversions
* Limit packet size to `MAX_SIZE`
* Use try/catch for network errors

---
