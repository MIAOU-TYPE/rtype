---
title: Overview
---

# Overview

This section documents the **server-side event system** used to decouple
networking, authentication, room management, and the simulation engine.

Events are **in-process signals**. They are not part of the client/server
protocol and are never serialized or sent over the network.

The goal of this layer is to make the server architecture composable,
maintainable, and robust under load.

---

## Why Events on the Server

The server coordinates multiple subsystems with very different concerns:

- TCP / UDP networking
- authentication and session management
- room lifecycle and membership
- game simulation (ECS and systems)
- persistence and metrics

Using events allows these systems to interact without tight coupling or
direct synchronous calls.

---

## What an Event Represents

Server events express either:

- **Facts**  
  Something that already happened  
  Example: `ClientDisconnected`, `SnapshotBuilt`

- **Intent**  
  A request to perform an action  
  Example: `JoinRoomRequested`, `LoginRequested`

Events describe *what*, not *how*.

---

## Event Flow

1. A subsystem publishes an event
2. The event is enqueued
3. The runtime drains the queue at controlled points
4. Subscribed handlers react

No event assumes a response. Events are disposable.

---

## Event Categories

### Network Events
Emitted by networking layers:
- client connected / disconnected
- TCP message received
- UDP datagram received

These events carry raw or minimally decoded data.

---

### Auth / Session Events
Emitted by auth and session services:
- login requested / succeeded / failed
- session expired
- UDP bind validated or rejected

These events gate access to gameplay systems.

---

### Room Events
Emitted by room management:
- room creation requested
- join accepted / rejected
- player left / kicked
- room destroyed

Room events are authoritative.

---

### Simulation Events
Emitted by the game engine:
- tick start / tick end
- snapshot built
- game start / game end

These events reflect the authoritative timeline.

---

## Threading Model

- Network IO typically runs on separate threads
- Network threads publish events into a thread-safe queue
- The runtime or simulation thread drains the queue
- Handlers that mutate game state run only on the simulation thread

No gameplay logic runs on network threads.

---

## Design Rules

- Events are strongly typed
- Event payloads are small and immutable
- No subsystem assumes who emitted an event
- Handlers must be fast and non-blocking
- Heavy work is delegated to owning subsystems

---

## Error Handling

- Exceptions in handlers are caught and logged
- A faulty handler must not crash the server
- Event dispatch continues for other subscribers

---

## Invariants

- Events are internal only
- Publishing is non-blocking
- Event handling has a well-defined execution context
- Events decouple subsystems without hiding authority
- The server remains stable under malformed or unexpected input
