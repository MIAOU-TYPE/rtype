---
id: network-protocol
title: Network Protocol
sidebar_label: Network Protocol
---

# **Network Protocol Specification**
# 1. Overview

This document defines the **binary protocol** used between the R-Type server and any native or third-party client.

The protocol is:

* **UDP-based**
* **Binary-encoded**
* **Little logic in the packet layer** — the server is authoritative
* **Fixed header**, followed by **packet-specific payload**
* **Big-endian (network order)** for all integers
* **Packed structures** (`#pragma pack(push,1)`)

Client messages are always sent to the server, which replies with acknowledgments and world-state updates.

---

# 2. General Packet Format

Every packet, regardless of type or direction, starts with this header:

```cpp
#pragma pack(push, 1)
struct PacketHeader {
    uint8_t  type;      // Protocol::X
    uint8_t  version;   // Always 1
    uint16_t size;      // Total packet size, in bytes (htons)
};
#pragma pack(pop)
```

### Header Fields

| Field       | Description                                                            |
| ----------- | ---------------------------------------------------------------------- |
| **type**    | Identifies the packet kind. Every message defines a unique type value. |
| **version** | Used for backward compatibility. Currently always `1`.                 |
| **size**    | `sizeof(packet)`, including header, encoded with `htons(size)`.        |

### Endianness Rules

| Type       | Encoding                                                                  |
| ---------- | ------------------------------------------------------------------------- |
| `uint8_t`  | Raw                                                                       |
| `uint16_t` | `htons()`                                                                 |
| `uint32_t` | `htonl()`                                                                 |
| `float`    | IEEE 754, bitwise copied (no endian conversion) unless custom rules apply |
| Structs    | Packed with `#pragma pack(push, 1)`                                       |

---

# 3. Protocol Types

A recommended enumeration (developers must keep client/server in sync):

```cpp
namespace Protocol {
    enum : uint8_t {
        CONNECT = 1,
        INPUT = 2,
        PING = 3,
        DISCONNECT = 4,

        ACCEPT = 10,
        REJECT = 11,
        PONG = 12,
        GAMEOVER = 13,

        ENTITY_CREATE = 20,
        ENTITY_DESTROY = 21,
        DAMAGE_EVENT = 22,
        SNAPSHOT = 23
    };
}
```

---

# 4. Client → Server Packets

## 4.1 CONNECT

Sent once when a client appears on the network.

```cpp
#pragma pack(push, 1)
struct PacketConnect {
    PacketHeader header;      // type = CONNECT
    uint32_t clientId;        // htonl()
};
#pragma pack(pop)
```

### Meaning

* Announces a new client to the server.
* `clientId` is optional and may be 0 for auto-assignment.

---

## 4.2 INPUT

Sent every time the player provides input (keyboard/gamepad).

```cpp
#pragma pack(push, 1)
struct PacketInput {
    PacketHeader header;      // type = INPUT
    uint32_t entity;          // which controlled entity (htonl)
    float dx;                 // horizontal axis (-1 → 1)
    float dy;                 // vertical axis (-1 → 1)
    uint8_t shooting;         // 1 = shoot, 0 = no
};
#pragma pack(pop)
```

### Behavior

* Server replaces the entity’s InputComponent.
* Shooting is handled by `ShootingSystem`.

---

## 4.3 PING

Used for latency measurement.

```cpp
#pragma pack(push, 1)
struct PacketPing {
    PacketHeader header;      // type = PING
};
#pragma pack(pop)
```

---

## 4.4 DISCONNECT

Requests server-side logout and entity cleanup.

```cpp
#pragma pack(push, 1)
struct PacketDisconnect {
    PacketHeader header;      // type = DISCONNECT
};
#pragma pack(pop)
```

---

# 5. Server → Client Packets

## 5.1 ACCEPT

```cpp
#pragma pack(push, 1)
struct PacketAccept {
    PacketHeader header;      // type = ACCEPT
};
#pragma pack(pop)
```

The server acknowledges a successful CONNECT.

---

## 5.2 REJECT

Connection failure (e.g. server full).

```cpp
#pragma pack(push, 1)
struct PacketReject {
    PacketHeader header;      // type = REJECT
};
#pragma pack(pop)
```

---

## 5.3 PONG

Response to PING.

```cpp
#pragma pack(push, 1)
struct PacketPong {
    PacketHeader header;      // type = PONG
    uint32_t timestamp;       // echoed timestamp (htonl)
};
#pragma pack(pop)
```

---

## 5.4 GAMEOVER

Sent when the player's entity is destroyed.

```cpp
#pragma pack(push, 1)
struct PacketGameOver {
    PacketHeader header;      // type = GAMEOVER
};
#pragma pack(pop)
```

---

## 5.5 ENTITY_CREATE

```cpp
#pragma pack(push, 1)
struct PacketEntityCreate {
    PacketHeader header;      // type = ENTITY_CREATE
    uint32_t id;              // htonl
    float x;
    float y;
    uint16_t sprite;          // htons
};
#pragma pack(pop)
```

---

## 5.6 ENTITY_DESTROY

```cpp
#pragma pack(push, 1)
struct PacketEntityDestroy {
    PacketHeader header;      // type = ENTITY_DESTROY
    uint32_t id;              // htonl
};
#pragma pack(pop)
```

---

## 5.7 DAMAGE_EVENT

```cpp
#pragma pack(push, 1)
struct PacketDamage {
    PacketHeader header;      // type = DAMAGE_EVENT
    uint32_t id;              // entity taking damage
    uint16_t amount;          // damage value (htons)
};
#pragma pack(pop)
```

---

## 5.8 SNAPSHOT (variable-size)

```cpp
#pragma pack(push, 1)
struct SnapshotEntity {
    uint32_t id;          // htonl
    float x;
    float y;
    float vx;
    float vy;
    uint16_t sprite;      // htons
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PacketSnapshot {
    PacketHeader header;      // type = SNAPSHOT
    uint32_t tick;            // server tick counter
    uint16_t entityCount;     // htons
    SnapshotEntity entities[]; // variable length
};
#pragma pack(pop)
```

### Encoding rule

`size = sizeof(PacketSnapshot) + entityCount * sizeof(SnapshotEntity)`.

---

# 6. Factory Mapping for Packet Creation

These map 1:1 to protocol messages:

```cpp
class PacketFactory {
public:
    static IServerPacket makeConnect(uint32_t clientId);
    static IServerPacket makeInput(uint32_t id, float dx, float dy, bool shooting);
    static IServerPacket makePing();
    static IServerPacket makeDisconnect();

    static IServerPacket makeAccept();
    static IServerPacket makeReject();
    static IServerPacket makePong(uint32_t timestamp);
    static IServerPacket makeGameOver();

    static IServerPacket makeEntityCreate(uint32_t id, float x, float y, uint16_t sprite);
    static IServerPacket makeEntityDestroy(uint32_t id);
    static IServerPacket makeDamage(uint32_t id, uint16_t amount);

    static IServerPacket makeSnapshot(uint32_t tick, const std::vector<SnapshotEntity>& entities);
};
```

---

# 7. Hex Example: ENTITY_CREATE

Suppose:

```
id = 42
x = 100.0f
y = 200.0f
sprite = 3
```

### Structure

```
Header:
  type      = 20  (ENTITY_CREATE)
  version   = 1
  size      = htons(??)

Payload:
  id        = htonl(42)
  x         = 42 C8 00 00 (depends on IEEE754)
  y         = C8 43 00 00
  sprite    = htons(3)
```

Full hex example:

```
14 01 00 14
00 00 00 2A
42 C8 00 00
43 48 00 00
00 03
```

---

# 8. Error Handling Rules

A client MUST treat packets as invalid when:

### Wrong version

If `header.version != 1`, packet must be ignored.

### Declared size mismatch

If `header.size != received_bytes`: ignore packet.

### Unknown packet type

Client should ignore silently (forward compatibility).

### Lifetime of variable packets

Snapshot packets must validate that:

```
header.size == sizeof(PacketSnapshot) + entityCount * sizeof(SnapshotEntity)
```

---

# 9. Third-Party Client Implementation Checklist

A third-party developer must:

### ✔ Accept UDP datagrams

### ✔ Implement packet deserialization

### ✔ Track world state based on:

* ENTITY_CREATE
* ENTITY_DESTROY
* DAMAGE_EVENT
* SNAPSHOT (full sync)

### ✔ Serialize input packets on key events

### ✔ Send periodic PING packets

### ✔ Handle disconnections gracefully

### ✔ Keep an entity table indexed by ID

### ✔ Render based on (x, y, sprite)

### ✔ Support interpolation/extrapolation for smoother motion

(snapshots arrive at 20–30 Hz, simulation runs at 60 Hz)

---

# 10. Summary Table

| Packet         | Direction | Purpose                 |
| -------------- | --------- | ----------------------- |
| CONNECT        | C → S     | Join server             |
| INPUT          | C → S     | Player input            |
| PING           | C → S     | Latency measurement     |
| DISCONNECT     | C → S     | Graceful exit           |
| ACCEPT         | S → C     | Connection accepted     |
| REJECT         | S → C     | Connection refused      |
| PONG           | S → C     | Ping response           |
| GAMEOVER       | S → C     | Player entity destroyed |
| ENTITY_CREATE  | S → C     | Spawn entity            |
| ENTITY_DESTROY | S → C     | Remove entity           |
| DAMAGE_EVENT   | S → C     | Entity takes damage     |
| SNAPSHOT       | S → C     | Full world state        |
