---
title: Event Bus
---

# Server Event Bus

The server event bus is an internal messaging layer used to decouple subsystems
(networking, room management, simulation, auth/session services). It provides a
predictable way to publish “facts” and “intent” inside the server without
creating tight dependencies or synchronous call chains.

This is not the network protocol. These events are **in-process** only.

---

## Purpose

The event bus exists to:

- isolate subsystems (network ↔ rooms ↔ simulation ↔ auth)
- reduce direct coupling and circular dependencies
- provide a single integration surface for side effects
- make server behavior easier to test and reason about

Events describe:
- something that happened (facts)
- a request to perform an action (intent)

---

## Minimal API

A minimal interface is sufficient:

- `publish(event)`  
  Enqueue an event for delivery.

- `subscribe<T>(handler)`  
  Register a handler for an explicit event type.

- `unsubscribe(token)` *(optional)*  
  Remove a handler.

Events are type-based, not string-based.

---

## Delivery Model

### Queue-based dispatch
- `publish()` does not synchronously call handlers
- events are queued
- the runtime drains the queue at controlled points (typically tick boundaries)

This prevents re-entrancy issues and unbounded synchronous chains.

### Deterministic ordering
- ordering is deterministic within a single queue
- cross-thread ordering is defined by enqueue time into the main queue

---

## Event Design Guidelines

- Prefer explicit event types over generic payloads
- Keep payloads small, copyable, and serializable (for logging/testing)
- Avoid passing ownership of heavy objects through events
- Events must be immutable after creation
- Use IDs and handles, not raw pointers

---

## Typical Server Event Categories

### Network events
- `Net.ClientConnected`
- `Net.ClientDisconnected`
- `Net.TcpMessageReceived`
- `Net.UdpDatagramReceived`

### Auth / session events
- `Auth.LoginRequested`
- `Auth.LoginSucceeded`
- `Auth.LoginFailed`
- `Auth.SessionExpired`
- `Auth.UdpBindSucceeded`

### Room events
- `Room.CreateRequested`
- `Room.JoinRequested`
- `Room.JoinAccepted`
- `Room.Left`
- `Room.Destroyed`

### Simulation events
- `Sim.TickStarted(roomId)`
- `Sim.TickEnded(roomId)`
- `Sim.SnapshotReady(roomId, seq)`

These categories prevent mixing concerns and keep handlers simple.

---

## Threading Model

Servers typically receive network IO on separate threads.

Rules:
- Network threads must not execute gameplay logic directly
- Network threads publish into a **thread-safe queue**
- The main simulation/runtime thread drains that queue during its update step

Handlers that mutate room or world state must run on the simulation thread.

---

## Error Handling

- Exceptions inside event handlers must be contained
- Faulty handlers should be logged and isolated
- The bus must continue dispatching remaining events

A single bad handler must not crash the server.

---

## Performance and Backpressure

- Event queues must be bounded or monitored
- Under overload, prefer dropping low-priority events (metrics, debug) rather than growing memory
- Keep handler work small; heavy work should be scheduled into the appropriate subsystem

---

## Invariants

- Publishing is non-blocking
- Handlers run in a known execution context (usually the runtime/simulation thread)
- Events are immutable and type-safe
- Event processing does not create unbounded synchronous chains
- Subsystems communicate through events, not direct cross-calls
