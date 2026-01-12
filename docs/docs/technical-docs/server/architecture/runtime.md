---
title: Runtime
---

# Server Runtime

The server runtime is the **process orchestrator**. It owns the application lifecycle, starts and stops
subsystems in a strict order, and guarantees safe shutdown. It must remain robust: failures in one client
or one room must not bring down the entire server.

This document describes responsibilities, startup/shutdown ordering, threading, and integration points.

---

## Responsibilities

The runtime is responsible for:

- Initialize core services (logging, configuration, storage)
- Initialize networking (TCP + UDP) and bind to ports
- Initialize authentication/session services
- Initialize room/instance management
- Start worker threads and main loops
- Monitor health and trigger controlled shutdown
- Ensure shutdown is **idempotent** and leak-free

The runtime does not implement gameplay rules; it only hosts and schedules the simulation.

---

## Startup Sequence

Startup must be deterministic and fail-fast.

Recommended order:

1. **Load configuration**
    - ports, tick rate, limits, database path
    - feature flags (compression, metrics, etc.)

2. **Initialize logging**
    - structured logs
    - per-subsystem prefixes

3. **Initialize persistence**
    - database open/migrations (if applicable)

4. **Initialize auth/session services**
    - password verification backend
    - session/token manager

5. **Initialize room manager**
    - default lobby state
    - capacity constraints
    - initial room registry

6. **Initialize networking**
    - create TCP listener / accept loop
    - create UDP socket
    - configure non-blocking mode
    - apply socket options (reuseaddr, buffers)

7. **Spawn worker threads**
    - TCP accept + per-client receive/write (or event loop)
    - UDP receive loop
    - simulation/tick loop per room (or centralized scheduler)
    - snapshot broadcast loop (optional: merged with tick)

8. **Enter main supervision loop**
    - monitor stop flag
    - periodic health checks / metrics
    - graceful shutdown on signal

### Failure handling
If any step fails:
- stop initialization
- close partially initialized resources
- exit with a clear error

No background thread may remain alive after a startup failure.

---

## Threading Model

The runtime typically separates concerns:

- **TCP threads**
    - accept new connections
    - read frames and route control messages
    - enqueue outgoing responses
    - disconnect handling and cleanup

- **UDP thread**
    - receive datagrams
    - validate bind/session association
    - enqueue decoded input commands per room

- **Simulation threads**
    - run fixed-tick updates
    - execute ECS systems
    - produce snapshot data

- **Broadcast thread** (optional)
    - send snapshots at a fixed rate
    - apply rate limiting / bandwidth caps

A minimal server can merge simulation + broadcast in one tick thread, but must never block on socket IO.

---

## Scheduling and Tick Rate

The simulation must be time-driven:

- Fixed step tick (e.g., 60 Hz or configurable)
- Independent of client message timing
- Inputs are sampled from queues each tick

Recommended pattern:
- drain input queue for each room at start of tick
- apply inputs
- run systems in stable order
- produce snapshot(s)
- schedule snapshot send

---

## Subsystem Integration Points

### Room manager
- runtime creates and owns the room registry
- networking routes join/leave/create/list to the room manager
- simulation executes per-room state

### Auth/session manager
- runtime owns auth storage and token issuance
- TCP messages trigger auth flows
- UDP bind validation queries session manager

### Networking
- TCP controls authoritative session membership and room state
- UDP carries real-time inputs and snapshots only after binding

---

## Shutdown Sequence

Shutdown is **idempotent** and safe to call multiple times.

Recommended order:

1. Set global stop flag
2. Stop accepting new TCP connections
3. Notify worker threads to exit
4. Join threads (in a defined order)
5. Close sockets and release networking resources
6. Flush logs and close persistence (DB)
7. Exit process cleanly

### Best-effort notifications
Optionally:
- send disconnect notifications to connected clients (TCP)
- broadcast room shutdown events

These are best-effort and must not block shutdown.

---

## Failure Containment

The runtime must contain failures:

- Malformed packets must not crash the server
- Per-client errors must only disconnect that client
- Per-room errors should, when possible, isolate to the room:
    - end the room
    - notify members
    - keep server process alive

Unhandled exceptions inside worker threads must be caught at the thread boundary and reported.

---

## Observability (Minimal)

Recommended runtime signals:
- startup configuration summary
- bound TCP/UDP ports
- number of connected clients
- number of active rooms
- tick rate / drift warnings
- snapshot send rate and packet drop stats (if tracked)

---

## Invariants

- Startup is deterministic and fail-fast
- Shutdown is idempotent and leak-free
- Simulation runs on fixed ticks independent of clients
- Socket IO never blocks simulation progress
- Auth and UDP binding gate all real-time traffic
- One client or room failure does not crash the process
