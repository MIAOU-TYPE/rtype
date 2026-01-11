---
title: Event Bus
---

# Event Bus

The event bus is the **decoupling backbone** of the client architecture. It allows
systems to communicate without direct dependencies while preserving ordering
and thread-safety guarantees.

---

## Purpose

The event bus enables:
- loose coupling between subsystems
- clear ownership boundaries
- predictable event flow across frames

No system should call into another system directly for cross-cutting concerns.

---

## Minimal API

A minimal, sufficient interface:

- `publish(event)`  
  Enqueue an event for later delivery.

- `subscribe<T>(handler)`  
  Register a handler for a specific event type.

- `unsubscribe(token)` *(optional)*  
  Remove a previously registered handler.

Subscriptions are type-based, not string-based.

---

## Event Design Guidelines

- Use explicit event types (`AuthLoginRequested`, `RoomJoined`)
- Keep payloads small, immutable, and copyable
- Avoid embedding large objects or ownership-transfer semantics
- Events describe **what happened**, not **what to do**

This makes events easy to reason about and safe to queue.

---

## Delivery Model

- Events are not dispatched immediately on `publish`
- Events are queued and dispatched during a controlled update phase
- Dispatch order is deterministic within a frame

This prevents deep synchronous call chains and re-entrancy issues.

---

## Threading Model

When background threads are present (e.g. networking):

- Background threads publish into a **thread-safe queue**
- The main thread drains that queue during the update step
- Handlers always execute on the main thread

No UI, rendering, or gameplay code runs on background threads.

---

## Error Handling

- Exceptions inside handlers must not crash the bus
- Faulty handlers should be isolated and logged
- Event dispatch must continue for other subscribers

---

## Invariants

- Event publication is non-blocking
- Handlers never assume execution context beyond “main thread”
- Event bus does not encode business logic
- Ordering is explicit and frame-bound

---
