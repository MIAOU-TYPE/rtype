---
title: Graphics Overview
---

# Graphics Overview

The graphics subsystem is responsible for transforming **client-side state**
into pixels. It is a pure presentation layer: it renders what it is given and
never alters gameplay or network state.

---

## Responsibilities

The graphics module is responsible for:

- Creating and managing the window and render context
- Initializing the rendering backend
- Loading and managing visual assets:
    - textures
    - sprites
    - fonts
    - shaders (if applicable)
- Exposing a render API consumed by the client runtime
- Presenting frames at a stable cadence

---

## Integration Model

- The runtime prepares render data for the current frame
- The graphics subsystem consumes this data as **read-only**
- Rendering has no visibility into:
    - networking
    - server authority
    - simulation logic

The graphics layer must remain stateless across frames, aside from cached
resources.

---

## Data Ownership and Safety

- Rendering consumes a snapshot of the state for the current frame
- All render data is immutable during rendering
- No render call mutates world, ECS, or UI logic
- Frame data exchange must be explicitly synchronized

This allows rendering to be reasoned about independently of simulation.

---

## Typical Render Pipeline

A standard frame follows this sequence:

1. Clear frame buffer
2. Draw background and parallax layers
3. Draw world entities (sorted by layer or Z-order)
4. Draw transient effects:
    - particles
    - explosions
    - flashes
5. Draw UI overlay
6. Present frame to screen

The exact implementation may vary, but ordering must remain consistent.

---

## Performance and Constraints

- Rendering must not block on:
    - disk IO
    - network IO
    - asset loading
- All assets required for a frame must already be loaded
- Expensive state changes should be minimized between draw calls

---

## Invariants

- Rendering never mutates gameplay state
- Rendering never blocks simulation or input
- A missing or invalid asset must not crash the client
- Visual output reflects the latest committed frame snapshot

---
