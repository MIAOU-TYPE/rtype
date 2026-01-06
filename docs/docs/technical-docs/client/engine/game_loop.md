# Game Loop

## Objectives

A real-time networked client must:

1. Stay responsive to player input.
2. Render smoothly (stable pacing).
3. Receive and process network packets continuously.
4. Apply server-driven world updates and render the latest view.

In your implementation, these goals are achieved by **splitting responsibilities across 3 loops**:

* **Display loop**: window events + UI/state update + rendering
* **Receiver loop**: non-blocking packet ingestion
* **Updater loop**: fixed-step world update + snapshot/commands consumption + render-command building

---

## High-level runtime structure

### Threads and responsibilities

**Main thread**

* Creates graphics + UDP client
* Starts `ClientRuntime`, then runs the display loop
* Waits for shutdown

**ClientRuntime threads**

* `_receiverThread`: continuously calls `receivePackets()`
* `_updaterThread`: runs a fixed-tick update loop (60 Hz target)

This separation avoids “render stutters” when network I/O spikes, and keeps world updates time-bounded.

---

## Display loop (UI + render pacing)

The display loop aims for a stable tick (`16ms`) and does:

1. Poll OS/window events → push them into `EventBus`
2. Dispatch the event bus (handlers update input state, close requests, etc.)
3. Update the current state (`StateManager`) using **frame input** (`InputState::consumeFrame()`)
4. Render:

    * draw render-commands produced by the updater thread
    * render the active UI/state
5. Sleep until next tick (with drift correction)

**Why it’s good**

* UI stays responsive because event polling and state updates happen every tick.
* Rendering uses **double-buffered render commands** (`_readRenderCommands` / `_writeRenderCommands`) to avoid blocking on world updates.

---

## Receiver loop (network ingestion)

The receiver thread is intentionally simple:

* Call `_client->receivePackets()` repeatedly
* Sleep ~1ms to avoid burning CPU

This fills the client’s internal packet queue so the updater can later drain it deterministically.

---

## Updater loop (fixed timestep + time-bounded work)

The updater thread runs at a 16ms cadence and tries to fit **network processing**, **command application**, and **world stepping** inside a “deadline” to avoid overruns.

### Core rules (as implemented)

* Target tick: `16ms`
* Fixed world step: `FixedDt = 1/60` seconds
* Clamp huge dt spikes: `MaxFrameDt = 0.16` seconds
* Prevent spiral of death: `MaxStepsPerTick = 4`
* Deadline per tick: `now + Tick - 1ms` (keeps safety margin)

### What happens each tick

1. Compute `frameDt` and add to `accumulator`
2. Drain network packets (bounded by time + count)
3. Apply ECS/world commands from a lock-free/queue buffer (bounded by time + count)
4. Step the world in fixed increments while:

    * `accumulator >= FixedDt`
    * `steps < MaxStepsPerTick`
    * `now < deadline`
5. Build render commands from ECS into `_writeRenderCommands`
6. Swap write → read (under mutex)
7. Sleep until next tick, and resync if drift is too large

**Why it helps**

* Heavy frames don’t cause infinite catch-up loops.
* You guarantee that network + command application remain bounded per tick.
* Rendering always consumes the latest prepared render commands.

---

## Input + events routing (how input becomes packets)

You use an `EventBus` + `EventRegistry` pattern:

* Window/input events are pushed by `pollEvents()`
* `EventBus::dispatch()` triggers handlers
* `ClientRuntime::setupEventsRegistry()` registers:

    * **KeyPressed handlers** that immediately `sendPacket(input)` for movement keys
    * **KeyReleased Space** to send “shoot” input
    * Generic listeners that update `InputState` (pressed/released/mouse)

This gives you two input paths:

1. **Immediate network inputs** (movement keys → packet send right away)
2. **Local UI/state input** via `InputState::consumeFrame()` for menu/UI interaction

---


### Client loop schema

```
┌────────────────────────────────────────────────────────────────────────────┐
│                               CLIENT RUNTIME                               │
│                                                                            │
│  Main thread: main()                                                       │
│   - create Graphics + UDPClient                                            │
│   - start ClientRuntime (spawns threads)                                   │
│   - runDisplay()                                                           │
│                                                                            │
│  ┌───────────────────────────────┐      ┌───────────────────────────────┐  │
│  │        Receiver Thread        │      │         Updater Thread        │  │
│  │   loop: receivePackets()      │      │  loop @ 16ms tick (fixed step)│  │
│  │   sleep(1ms)                  │      │                               │  │
│  │                               │      │  - drain packet queue         │  │
│  │  ┌─────────────────────────┐  │      │  - PacketRouter → WorldCommand│  │
│  │  │ UDPClient (socket UDP)  │  │      │  - apply cmmds to ClientWorld││  │
│  │  │ recvfrom → RX queue     │──┼──────►  - step world (FixedDt)       │  │
│  │  └─────────────────────────┘  │      │  - build RenderCommands (ECS) │  │
│  └───────────────────────────────┘      │  - swap write→read (mutex)    │  │
│                                         └───────────────▲───────────────┘  │
│                                                         │                  │
│                                          RenderCommands (read buffer)      │
│                                                         │                  │
│  ┌──────────────────────────────────────────────────────┴───────────────┐  │
│  │                         Display Loop (main thread)                   │  │
│  │  loop @ 16ms tick                                                    │  │
│  │   1) graphics.pollEvents() → EventBus                                │  │
│  │   2) EventBus.dispatch() → EventRegistry handlers                    │  │
│  │      - update InputState                                             │  │
│  │      - stop on Escape / WindowClosed                                 │  │
│  │      - (optional) send immediate input packets                       │  │
│  │   3) StateManager.update(InputState.consumeFrame())                  │  │
│  │   4) render: draw RenderCommands + render current State/UI           │  │
│  │   5) sleep_until(next tick) + drift correction                       │  │
│  └──────────────────────────────────────────────────────────────────────┘  │
│                                                                            │
└────────────────────────────────────────────────────────────────────────────┘
```

## Server snapshots and smoothing (where it fits)

In your codebase, “smoothing/interpolation” would live in the **world rendering pipeline**, not in the display loop:

* Updater thread:

    * receives packets → `PacketRouter` → writes `WorldCommand`s into `_commandBuffer`
    * applies commands to `ClientWorld`
    * steps the world
    * generates render commands from ECS (`RenderSystem::update`)

If you add interpolation:

* store previous/latest server state (or timestamps) in components
* generate interpolated transforms in `RenderSystem::update(...)` (or a dedicated interpolation system) before emitting render commands.

---

## Shutdown conditions

Shutdown is triggered by:

* state manager no longer running
* stop requested (e.g., Escape or window close)
* runtime stop sends a DISCONNECT packet and joins threads


