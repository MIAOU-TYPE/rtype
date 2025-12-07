# Networking – PacketFactory Module

## Overview

The `factory` module provides all logic required for **client-side packet creation and serialization**.
Its role is to produce valid binary packets that will be sent to the server through the UDP networking layer.

The module contains:

```
protocol/factory/
├── PacketFactory.hpp   # Public API & templates
├── PacketFactory.cpp   # Implementation
└── PacketFactory.tpp   # Template definitions (makePacket)
```

This factory is the **only place** in the client where packet structures (`InputData`, `HeaderData`, etc.) are assembled in memory before being sent.

---

# Responsibilities of the PacketFactory

The `PacketFactory` takes an existing `IPacket` instance (used as a raw buffer allocator) and is responsible for:

### ✔ Creating packet headers

Automatically fills:

* packet *type*
* packet *version*
* packet *total size*
* endian conversions (network order)

---

### ✔ Serializing packet payloads

Writes the full packet (header + data struct) into the underlying `IPacket` buffer.

All binary data structures come from:

* `HeaderData.hpp`
* `DefaultData.hpp`
* `InputData.hpp`
* `TypesData.hpp`
* `Endian.hpp`

---

### ✔ Producing ready-to-send packets

Returned packets always satisfy:

* correct size
* correct memory layout (packed)
* correct version
* fully serialized TCP/UDP-compatible buffer

---

# Class Overview: PacketFactory

📄 *File:* `PacketFactory.hpp`

The class exposes two main creation methods:

| Method                                | Purpose                                                                         |
| ------------------------------------- | ------------------------------------------------------------------------------- |
| `makeBase(flag)`                      | Build packets with only a header + simple value (ex: CONNECT, DISCONNECT, PING) |
| `makeInput(entity, dx, dy, shooting)` | Build a full *player input* packet                                              |

Internally, both rely on a private templated method `makePacket<Type>` defined in `PacketFactory.tpp`.

---
# 4. Internal Implementation Details

## 4.1 Header construction

```cpp
static HeaderData makeHeader(uint8_t type, uint16_t size) noexcept;
```

Automatically sets:

* version
* packet type
* size converted to network byte order
* no padding (packed struct)

---

## 4.2 Generic template creation (PacketFactory.tpp)

```cpp
template <typename Type>
std::shared_ptr<Net::IPacket>
makePacket(const Type &packetData) const;
```

This method:

1. Clones the base `_packet` to get a fresh writable buffer.
2. Copies the struct (`packetData`) directly in memory (`std::memcpy`).
3. Calls `setSize(sizeof(Type))`.
4. Returns a valid, ready-to-send packet.

The factory therefore only requires:

* the data struct to be trivially copyable
* proper usage of packed binary structures

No dynamic allocation is performed inside data structs.

---

# 5. Packet Lifecycle

1. The game requests an input packet:

   ```cpp
   factory.makeInput(entityId, dx, dy, shooting);
   ```

2. PacketFactory builds & serializes the binary buffer.

3. The networking layer sends the raw buffer using `sendTo()`.

4. The server receives and interprets it using the same binary layout.

---

# Summary Table

| File                  | Description                                        |
| --------------------- | -------------------------------------------------- |
| **PacketFactory.hpp** | Public interface, constructor, packet creation API |
| **PacketFactory.cpp** | Implementation of high-level methods               |
| **PacketFactory.tpp** | Template `makePacket<Type>` used for serialization |
| **HeaderData.hpp**    | PacketHeader layout                                |
| **DefaultData.hpp**   | Base command packet struct                         |
| **InputData.hpp**     | Player input struct                                |
| **TypesData.hpp**     | Packet type enumerations                           |
| **Endian.hpp**        | Endianness utilities                               |
&