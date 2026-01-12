---
title: Architecture Overview
---

# Architecture Overview

The client architecture is intentionally **modular, explicit, and defensive**.
Each subsystem:

* Owns its **memory**
* Controls its **execution model**
* Exposes only **narrow, intention-revealing interfaces**
* Never relies on implicit side effects from other subsystems

This design prioritizes:

* Predictable behavior under load
* Debuggability
* Safe partial initialization and teardown
* Clear thread-ownership boundaries

---

## Core Subsystems

### Runtime / Lifecycle

The Runtime subsystem is the **root authority** of the application.

**Responsibilities**

* Owns the global startup and shutdown sequence
* Initializes subsystems in strict dependency order
* Spawns, names, and joins worker threads
* Manages fatal and recoverable error paths

**Key properties**

* Subsystems may fail to initialize; Runtime must handle partial success
* Shutdown is always **ordered and idempotent**
* No subsystem is allowed to spawn threads outside Runtime control

**Typical flow**

1. Parse configuration
2. Initialize logging & diagnostics
3. Initialize networking
4. Initialize world/simulation
5. Initialize rendering, UI, audio
6. Enter main execution loop
7. On failure or exit: unwind in reverse order

---

### Networking

The Networking subsystem is strictly divided into **control plane** and **data plane**.

#### TCP Client (Control Plane)

* Connection lifecycle (connect, reconnect, disconnect)
* Reliable delivery of commands and configuration
* Authentication and session negotiation
* Heartbeats and connection health monitoring

#### UDP Client (Data Plane)

* High-frequency snapshot and state updates
* Best-effort delivery with application-level validation
* Sequence numbers and snapshot reconciliation
* Drop tolerance and out-of-order handling

**Shared responsibilities**

* Packet parsing and validation
* Routing packets into typed internal messages
* Enforcing back-pressure via bounded queues
* Zero gameplay logic

**Hard constraints**

* Networking never blocks simulation or rendering
* Networking never mutates world state directly
* All outputs are immutable messages

---

### World / Simulation

The World subsystem is the **authoritative client-side state machine**.

**Responsibilities**

* Apply server commands and state snapshots
* Maintain authoritative local simulation state
* Run a fixed-step update loop (e.g. 60 Hz)
* Perform prediction, reconciliation, and interpolation if required
* Produce a read-only, render-ready representation of the world

**Key invariants**

* Simulation step is deterministic given identical inputs
* No rendering or audio side effects
* No direct network access

**Outputs**

* Immutable world snapshots
* Event streams (e.g. entity created, destroyed, state changed)

---

### Rendering

The Rendering subsystem is a **pure consumer** of world state.

**Responsibilities**

* Translate world snapshots into render commands
* Own all GPU resources (buffers, textures, pipelines)
* Handle frame pacing and vsync
* Perform culling, batching, and GPU-side optimizations

**Constraints**

* Rendering never mutates gameplay or simulation state
* Rendering never blocks simulation
* Rendering operates on immutable data only

**Design goal**

> Rendering should be restartable or replaceable without affecting gameplay correctness.

---

### UI

The UI subsystem is a **presentation and input layer**, not a control authority.

**Responsibilities**

* Menu navigation and overlays
* Input collection and abstraction
* User feedback (HUD, notifications, errors)
* Emitting high-level intent events (e.g. “JoinGame”, “ChangeSettings”)

**Constraints**

* UI never calls networking APIs directly
* UI never mutates world state
* UI emits events, not commands

This separation allows:

* Headless execution
* Alternate UI implementations
* Automated testing of gameplay without UI

---

### Audio

The Audio subsystem is **event-driven and loosely coupled**.

**Responsibilities**

* Playback of sound effects and music
* Mixing, spatialization, and attenuation
* Resource management for audio assets

**Constraints**

* No direct dependency on world internals
* Consumes high-level audio events only
* No gameplay or simulation logic

This ensures audio failure or latency never affects core gameplay.

---

## Threading Model

A typical runtime configuration includes the following long-lived threads:

* **Render / UI Thread**
* **UDP Receive Thread**
* **TCP Receive Thread**
* **Update / Simulation Thread**

Each thread has:

* A single, clearly defined owner
* A single primary responsibility
* Explicit input and output channels

---

## Inter-Thread Communication

Threads communicate **exclusively** via:

* Lock-free queues or bounded blocking queues
* Immutable messages and data snapshots
* Event dispatch at well-defined synchronization points

**Rules**

* No shared mutable state
* No cross-thread method calls
* No implicit synchronization
* Ownership of data is always explicit

**Rationale**

* Eliminates data races by construction
* Simplifies reasoning about correctness
* Makes profiling and debugging tractable

---

## Design Principles (Summary)

* **Single ownership** of memory and execution
* **Explicit boundaries** between subsystems
* **Fail-safe startup and teardown**
* **Deterministic simulation**
* **Pure rendering and presentation layers**
* **Event-driven, message-passing concurrency**
