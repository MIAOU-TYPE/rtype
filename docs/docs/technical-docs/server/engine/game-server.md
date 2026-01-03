---
id: game-server
title: Game Server Architecture
sidebar_label: Game Server
---

## Overview

The `GameServer` is the authoritative controller of the server-side simulation.  
It is responsible for:

- Handling gameplay-related network events  
- Managing player entities  
- Owning and updating the game world  
- Dispatching updates to gameplay systems  
- Producing server-side responses (e.g., PONG packets)

It implements the `IMessageSink` interface, making it the endpoint for all gameplay-relevant messages parsed by the `UDPPacketRouter`.

---

## Responsibilities

### 1. World Ownership
The `GameServer` owns an instance of an `IGameWorld` implementation:

```cpp
std::unique_ptr<IGameWorld> _world;
````

This ensures that:

* The world exists as long as the game server exists.
* The world is immediately accessible to all gameplay systems.
* No external code manages or manipulates the world unintentionally.

---

### 2. Player Lifecycle

| Event                    | Behavior                                                             |
| ------------------------ | -------------------------------------------------------------------- |
| `onPlayerConnect(id)`    | Creates a new player entity in the world and stores the association. |
| `onPlayerDisconnect(id)` | Destroys the bound entity and removes the session mapping.           |

The mapping is stored internally:

```cpp
std::unordered_map<int, Ecs::Entity> _sessionToEntity;
```

---

### 3. Input Handling

The server receives decoded input from the `UDPPacketRouter` and updates the appropriate `InputComponent`:

```cpp
_world->registry()
      .getComponents<InputComponent>()[entity] = msg;
```

Inputs do **not** directly change positions.
They only update components consumed by gameplay systems.

---

### 4. Gameplay Update Loop

Each simulation tick, the server calls:

```cpp
InputSystem::update(*_world);
MovementSystem::update(*_world, dt);
```

The pattern is always:

1. **Read components**
2. **Transform state**
3. **Write components**

---

### 5. Network Communication

Although the gameplay engine is separate from the network layer, `GameServer` handles minimal networking responsibilities, such as responding to PING messages:

```cpp
if (auto pkt = _factory.makeDefault(..., PONG))
    _server->sendPacket(*pkt);
```

This keeps latency detection inside the server loop but retains a clean separation from gameplay logic.

---

## Interaction Diagram

```
UDPPacketRouter → GameServer → IGameWorld → ECS → Systems
                     ↑
                     │
                  Network
```

The `GameServer` is the *entry point* from the networking layer into the gameplay engine.