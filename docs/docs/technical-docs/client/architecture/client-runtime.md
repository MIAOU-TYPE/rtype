---
title: Client Runtime
---

The client runtime is the **orchestrator** of all client subsystems. It owns the application lifecycle,
spawns worker threads, routes events, and enforces shutdown ordering. It must remain free of gameplay
rules: authoritative gameplay logic lives on the server.

This document describes responsibilities, threading, ordering guarantees, and the concrete mapping to the
current implementation (`ClientRuntime`).

---

## 1. Purpose and Design Constraints

### Goals
- Provide a single entry point coordinating **graphics**, **audio**, **networking**, **world simulation**, and **UI state**
- Enforce **ordering rules** (startup, steady state, shutdown)
- Keep the client responsive while receiving network traffic continuously
- Ensure shutdown is **safe**, **idempotent**, and **resource-leak free**

### Non-goals
- Implement server-authoritative gameplay logic (spawns, damage, scoring, etc.)
- Guarantee delivery of UDP gameplay packets (UDP is best-effort by design)

---

## 2. Owned Subsystems

The runtime creates and owns (directly or indirectly) the following components:

### Rendering / Windowing
- `Graphics::IGraphics`
- Renderer instance (`_renderer`)
- Render command buffers:
  - `_readRenderCommands`
  - `_writeRenderCommands`

### Audio
- `_musicRegistry`
- `_soundRegistry`

### Event / Input
- `Engine::EventBus` (`_eventBus`)
- `Engine::EventRegistry` (`_eventRegistry`)
- `Engine::InputState` (`_input`)

### Game Presentation State
- `Engine::StateManager` (`_stateManager`)
- Current state instances:
  - Menu / Lobby state
  - Game state

### Networking
- UDP client (`_udpClient`) + `Ecs::UDPPacketRouter`
- TCP client (`_tcpClient`) + `Network::TCPPacketRouter`
- Packet factories:
  - `_udpPacketFactory`
  - `_tcpPacketFactory`

### World / ECS / Presentation Data
- `World::ClientWorld` (`_world`)
- `Engine::SpriteRegistry` (`_spriteRegistry`)
- `Engine::RoomManager` (`_roomManager`)
- `Engine::AuthContext` (`_authCtx`)

---

## 3. Threading Model

The runtime runs **three worker threads** plus the main thread (render loop):

- **Main thread**
  - Window events polling
  - Event dispatch
  - UI state update + rendering
  - Presentation logic / state transitions

- **TCP thread** (`runTcp`)
  - Receives TCP frames
  - Parses and routes them
  - Performs handshake (HELLO loop until connected)
  - Handles `AUTH_OK`, room list updates, game start signals

- **UDP receiver thread** (`runReceiver`)
  - Receives UDP datagrams
  - Enqueues packets into UDP RX buffer (client networking layer)

- **Updater thread** (`runUpdater`)
  - Fixed tick stepping for client world simulation
  - Applies world commands produced by network input/events
  - Builds render commands for the renderer
  - Maintains frame timing and drift correction

### Shared-state synchronization
- `_frameMutex` protects swapping `_readRenderCommands` and `_writeRenderCommands`
- `_mutex` + `_cv` used to block `wait()` until stop is requested
- `_stopRequested` and `_running` are atomic guards for lifecycle
- `_authCtx->m` protects auth context fields; `authErrorVersion` provides a cheap “changed” signal

---

## 4. Ordering and Ownership Rules

### Ordering rules (high-level)
1. Create graphics + renderer first (needed by UI states)
2. Create event bus / registries before starting threads (threads will publish side effects used by UI)
3. Create packet routers and packet factories before networking begins
4. Start TCP before UDP (TCP provides connection metadata / welcome handshake used to bind UDP)
5. UI loop (`runDisplay`) must remain on a single thread (graphics backends are typically not thread-safe)

### Ownership rules
- `ClientRuntime` owns thread lifetimes and is the only component allowed to:
  - start threads
  - stop threads
  - close sockets
  - destroy graphics backend
- Subsystems may *request* actions through events but must not shut down the runtime directly.

---

## 5. Startup Sequence (Concrete)

### 5.1 Construction phase
Performed in `ClientRuntime::ClientRuntime(...)`:

1. Create window + renderer  
   `create(Extent2u{1280, 720}, "R-Type", false)`  
   `_renderer = _graphics->createRenderer()`

2. Create event infrastructure  
   `_eventBus`, `_eventRegistry`

3. Create networking routers + factories  
   `_udpPacketFactory`, `_tcpPacketFactory`  
   `_udpPacketRouter`, `_tcpPacketRouter`

4. Create gameplay presentation systems  
   `_spriteRegistry`, `_world`, `_stateManager`  
   `_authCtx`, `_roomManager`

5. Create audio registries and load assets  
   `_musicRegistry`, `_soundRegistry`  
   `AssetLoader::load(...)`

6. Enter initial state  
   Menu state is installed at the end of the constructor.

### 5.2 Start phase
Performed in `ClientRuntime::start()`:

1. Start TCP client (`_tcpClient->start()`)
2. Start UDP client (`_udpClient->start()`)
3. Mark runtime running (`_running = true`)
4. Register handlers
   - `setupGlobalEventHandlers()`
   - `setupEventsRegistry()`
5. Spawn worker threads
   - `_tcpThread = runTcp`
   - `_receiverThread = runReceiver`
   - `_updaterThread = runUpdater`

### 5.3 Render loop entry
The main thread calls `runDisplay()`:
- Executes the render loop until `_running` is false or `StateManager` stops

### Startup failure handling
If any exception occurs during start:
- `stop()` is called
- the exception is re-thrown

This ensures partial startup does not leave sockets or threads alive.

---

## 6. Runtime Steady-State Behavior

### 6.1 Event routing
- Low-level input events (`KeyPressed`, `MouseMoved`, etc.) update `InputState`
- High-level UI actions become domain events:
  - Create room / join room / list rooms
  - Register / login requests

These are converted into TCP packets using `TCPPacketFactory` and sent via `_tcpClient`.

### 6.2 TCP processing and signals
The TCP thread:
- Subscribes handlers on the TCP message sink:
  - `WELCOME`: triggers UDP connect using server-provided connect info
  - `ROOMS_LIST`: updates room manager data
  - `GAME_START`: sets `_pendingGameStart`
  - `AUTH_OK`: updates `AuthContext`, sets `_pendingAuthOk`

The main thread observes `_pendingGameStart` / `_pendingAuthOk` and performs state transitions.

### 6.3 UDP processing
UDP receive thread:
- pulls datagrams via `_udpClient->receivePackets()`

Updater thread:
- calls `processNetworkPackets(...)` which pops UDP packets and hands them to `_udpPacketRouter`

### 6.4 World stepping and render commands
The updater thread:
- runs a fixed-step simulation (`FixedDt = 1/60`)
- applies commands from `_commandBuffer` into `_world`
- builds render commands via `RenderSystem::update(...)`
- swaps command buffers under `_frameMutex`

The main thread renders the latest `_readRenderCommands`.

---

## 7. Shutdown Sequence (Concrete)

Shutdown is **idempotent** via `_stopRequested.exchange(true)` in `stop()`.

### 7.1 Stop signaling
- `_running = false`
- `_cv.notify_all()` unblocks `wait()`

### 7.2 Best-effort disconnect packets
- Send TCP leave room packet (if possible)
- Send UDP disconnect packet (if possible)

These sends are explicitly best-effort and must not block shutdown.

### 7.3 Join worker threads
- join TCP thread
- join UDP receiver thread
- join updater thread

Join order is chosen to minimize races:
- once `_running` is false, threads will exit their loops
- join ensures no background access to networking/graphics remains

### 7.4 Close networking resources
- `_tcpClient->close()`
- `_udpClient->close()`

### 7.5 Graphics shutdown
- `runDisplay()` ends by calling `_graphics->close()` and then `stop()`
- Destructor additionally calls `stop()` in a try/catch

### 7.6 Partial initialization guarantee
A partially constructed runtime must still be safe to destroy:
- `stop()` must tolerate missing threads / disconnected sockets
- `close()` methods must tolerate already-closed sockets
- no join must be attempted on a non-joinable thread

---

## 8. Error Handling and Robustness Rules

### Non-fatal errors
- Protocol parsing errors should not crash the runtime
- Unknown TCP packet types must be reported via protocol error sink but ignored safely

### Fatal errors
The runtime initiates shutdown when:
- graphics backend closes
- critical exceptions escape thread loops (should be prevented by catching internally)
- network client closes unexpectedly and the state manager decides to stop (policy-specific)

### Recommended invariants
- No thread must access `_graphics` or `_renderer`
- All shared mutable state is either:
  - guarded by mutex, or
  - updated atomically, or
  - updated by one thread and consumed by another using atomic flags/versioning

---

## 9. Extensibility Guidelines

When adding features, preserve these invariants:

- Never block the render loop on network IO
- Keep all socket IO on TCP/UDP threads
- Keep gameplay authoritative on the server
- Prefer event-driven requests (UI → event → packet) over direct subsystem calls
- Keep shutdown idempotent: new resources must be closed or joined in `stop()`

Typical safe extensions:
- `LOGOUT` / `RESUME_SESSION` flows in TCP thread
- Reconnection policy state machine
- Network metrics (ping, packet loss estimation) surfaced to UI via events
- Render command batching optimizations (still swap-buffered)

---
```
