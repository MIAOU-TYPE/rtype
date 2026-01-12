---
title: Game Loop
---

# Server Game Loop

The server game loop is the **authoritative simulation loop**. It advances game state on a fixed timeline,
applies validated client inputs, executes ECS systems in a stable order, and produces snapshot data for replication.

This loop must be deterministic in structure, resilient to lag or packet loss, and independent from rendering or UI
(the server does not render).

---

## Goals

- Run simulation at a fixed tick rate (e.g. 60 Hz, configurable)
- Apply client inputs consistently and fairly
- Keep system execution order stable
- Generate authoritative snapshots at a controlled cadence
- Remain robust under load (do not spiral on slow frames)

---

## Fixed Timestep Model

The loop runs with a fixed timestep `dt`:

- `tickRateHz` (configurable)
- `dt = 1 / tickRateHz`

Timekeeping must be monotonic (steady clock).

### Drift handling
- If the process wakes up late, it should not run unbounded catch-up.
- Cap maximum catch-up ticks per cycle (e.g. 4).
- If the server falls behind repeatedly, prefer skipping extra catch-up and logging a warning.

This prevents “death spirals” under high load.

---

## Loop Stages (Per Tick)

A single tick is conceptually split into stable stages:

1. **Drain input queues**
    - Collect all pending inputs for each room
    - Validate and normalize inputs
    - Apply only inputs belonging to authenticated, bound clients

2. **Apply inputs**
    - Convert inputs into component updates or command buffers
    - Enforce rate limits (shoot cooldown, movement caps) server-side

3. **Run simulation systems**
   Execute systems in a deterministic order, for example:
    - movement and physics
    - collisions and damage
    - spawns / waves
    - AI
    - lifetime / cleanup
    - scoring and win conditions

4. **Update room state**
    - Check game phase transitions (lobby → in-game → ended)
    - Resolve joins/leaves/kicks that were confirmed over TCP
    - Ensure per-room invariants (capacity, membership)

5. **Build replication output**
    - Produce snapshot data for each bound client
    - Include a snapshot `sequence` number
    - Optionally compress or delta-encode (design-dependent)

6. **Schedule snapshot send**
    - Push snapshots to UDP send queues
    - Sending may be performed immediately or by a dedicated broadcast loop

---

## Per-Room vs Global Loop

Two valid models:

### Per-room loop
- Each room runs its own tick thread
- Simplifies isolation and timing per instance
- Requires careful scaling limits

### Global scheduler
- One loop ticks all rooms
- Easier to manage CPU usage and instrumentation
- Requires careful per-room time budgeting

Both models must preserve the same tick-stage ordering.

---

## Input Semantics

Inputs arriving over UDP are:
- best-effort
- possibly duplicated
- possibly out of order

Rules:
- Inputs must carry a sequence or timestamp
- The server may drop stale inputs
- The server must never block waiting for inputs
- Missing inputs imply “no input” for that tick

Validation:
- reject inputs from unbound endpoints
- enforce rate limits and bounds
- sanitize malformed payloads

---

## Snapshot Semantics

Snapshots are authoritative and time-sensitive.

Rules:
- Each snapshot has a monotonic `sequence`
- Older snapshots must never replace newer state client-side
- Snapshots may be broadcast at:
    - every tick, or
    - a lower frequency (e.g. 20–30 Hz) while simulation runs at 60 Hz

The snapshot rate is a bandwidth/performance trade-off.

---

## Load Shedding and Backpressure

Under overload:
- cap catch-up ticks per cycle
- cap per-tick processing time per room (budgeting)
- drop outgoing UDP snapshots if send queues overflow
- preserve simulation correctness over delivery guarantees

Never allocate unbounded memory for queues.

---

## Room Transitions

Room lifecycle events should not disrupt simulation:

- Join/leave decisions are confirmed via TCP, then applied at a tick boundary
- Starting the game is a server-controlled transition
- Ending the game produces final snapshot/state and returns to lobby or destroys room

On room switch:
- reset per-room snapshot sequence (or use global sequence but per-room mapping)
- clear per-client interpolation assumptions (client responsibility)

---

## Error Handling

- All tick stages must be exception-safe at boundaries
- Failure in one entity/system should not corrupt the whole room
- If a room becomes invalid:
    - end the room safely
    - notify clients via TCP best-effort
    - keep the server process alive

---

## Invariants

- Simulation progresses on fixed ticks independent of clients
- System order is stable and deterministic in structure
- Inputs are validated and bounded
- Snapshot output derives from authoritative state only
- Overload does not cause unbounded catch-up or memory growth
