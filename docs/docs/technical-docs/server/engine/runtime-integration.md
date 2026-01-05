---
id: runtime-integration
title: Runtime Integration
sidebar_label: Runtime Integration
---

## Overview

The game engine operates inside the server runtime loop.  
Two threads participate in gameplay processing:

1. **Receiver thread** — reads incoming UDP packets  
2. **Processor thread** — processes packets and runs gameplay updates  

---

## Packet Flow

````

UdpServer → UDPPacketRouter → GameServer → World → Systems

````

### Step-by-step:

1. `UdpServer` receives a UDP packet  
2. Packet is wrapped in an `IServerPacket` and queued  
3. `ServerRuntime::runProcessor()` pops the packet  
4. `UDPPacketRouter` validates and interprets the data  
5. `UDPPacketRouter` calls the appropriate `IMessageSink` method  
6. `GameServer` updates world state  
7. `GameServer.update(dt)` runs gameplay systems  

---

## Update Loop

`ServerRuntime` computes `dt` on every tick:

```cpp
auto now = steady_clock::now();
float dt = duration<float>(now - last).count();
last = now;

_gameServer->update(dt);
````

This design ensures:

* deterministic ordering
* real-time gameplay simulation
* decoupling between network and game logic

---

## Why GameServer Owns the World

The runtime only manages:

* threads
* network IO
* routing packets

The gameplay logic, including world state, is intentionally isolated inside `GameServer`.

This separation improves:

* modularity
* maintainability
* testability
* clarity in architecture

---

## Extensibility

This design allows future improvements:

* multiple worlds (e.g., rooms or instances)
* hot-reloading world state
* simulation rewind and replay
* dedicated debug worlds for testing
* multiple game modes

The runtime loop remains unchanged while gameplay evolves separately.
