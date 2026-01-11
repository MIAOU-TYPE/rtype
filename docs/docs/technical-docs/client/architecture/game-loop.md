---
title: Game Loop
---

The client uses multiple coordinated loops rather than a single monolithic loop.
This avoids coupling rendering performance to network or simulation jitter.

## Loop separation

### Render loop

- Runs at target FPS (e.g. 60)
- Polls windowing events
- Dispatches UI and input events
- Consumes prebuilt render commands

### Update loop

- Runs at a fixed timestep
- Applies world commands
- Steps simulation deterministically
- Builds render command buffers

### Network polling

- Fully non‑blocking
- Bounded amount of work per tick
- Never allocates unbounded memory

## Fixed timestep rationale

A fixed timestep ensures:

- Deterministic simulation
- Stable collision and movement logic
- Predictable CPU usage

Catch‑up is explicitly capped to avoid spiral‑of‑death scenarios when frames are missed.

## Practical invariants

- Rendering never waits for simulation
- Simulation never waits for network
- Network never pushes unbounded work into simulation

Breaking any of these invariants results in visible hitching or instability.
