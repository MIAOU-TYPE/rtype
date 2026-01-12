---
title: Audio Implementation Notes
---

This document describes conventions and implementation guidelines for the audio subsystem.
The goal is to ensure **consistency**, **clarity**, and **low-latency feedback** while keeping
audio logic decoupled from gameplay rules.

---

## 1. Asset Naming and Identification

Audio assets are referenced by **stable, semantic identifiers** rather than file paths.
Identifiers must remain constant even if underlying files change.

### Naming rules
- Lowercase
- Dot-separated namespaces
- No file extensions

### Recommended namespaces
- `sfx.*` — short sound effects
- `music.*` — background music or long tracks
- `ui.*` — interface feedback (optional but recommended)

### Examples
- `sfx.shoot`
- `sfx.explosion`
- `sfx.hit`
- `ui.click`
- `music.menu`
- `music.ingame`

This allows:
- asset replacement without touching code
- centralized audio configuration
- easier debugging and profiling

---

## 2. Loading and Lifetime

### Preloading policy
- UI sounds and menu music should be preloaded at startup
- Frequently used gameplay SFX (shots, impacts) should be preloaded
- Large or rarely used assets may be loaded lazily

### Ownership
- The audio backend owns decoded buffers
- Callers only hold lightweight identifiers or handles
- No gameplay system should directly load audio files

---

## 3. Mixing and Concurrency Control

### Concurrent sound limits
To avoid audio spam and clipping:
- Enforce a maximum number of concurrent instances per SFX
- Typical limits:
    - `sfx.shoot`: low (e.g. 3–5)
    - `sfx.explosion`: medium (e.g. 2–3)
    - `ui.click`: very low (1)

When the limit is reached:
- either drop the new instance
- or replace the oldest / lowest-priority instance

### Priorities
Each sound instance has a priority:

Example priority order:
1. Critical gameplay feedback (explosions, player damage)
2. Primary actions (shooting, pickups)
3. UI feedback

Higher-priority sounds may:
- preempt lower-priority ones
- bypass stricter concurrency limits

---

## 4. Volume Groups and Controls

Audio is mixed through **volume groups**:

- `master`
- `music`
- `sfx`
- `ui` (optional)

### Rules
- Master volume scales all output
- Music and SFX volumes are independently adjustable
- UI sounds may share SFX volume or use a dedicated group

Volume changes:
- must apply immediately
- must not restart or glitch currently playing sounds

This enables:
- accessibility options
- dynamic balancing
- user customization

---

## 5. Latency and Synchronization

### Triggering rules
- Audio should be triggered in the **same frame** as the corresponding visual event
- Audio playback must not wait on:
    - network confirmation
    - disk IO
    - background threads

Typical examples:
- Shooting sound triggered when input is accepted locally
- Explosion sound triggered when explosion is rendered

### Network considerations
- The client may predictively play sounds for local actions
- Server corrections may suppress or override future sounds, but should not retroactively cancel audio already played

---

## 6. Threading and Safety

- Audio API calls must be safe to call from the main thread
- No audio backend calls from network threads
- Audio commands should be lightweight and non-blocking

If an audio backend uses its own internal thread:
- synchronization must be handled internally
- callers must not rely on timing guarantees beyond “as soon as possible”

---

## 7. Failure and Fallback Behavior

- Missing or invalid audio assets must not crash the client
- On failure:
    - log a warning
    - skip playback
- The game must remain fully playable without audio

---

## 8. Extensibility Guidelines

When adding new sounds:
- define identifier first
- assign:
    - volume group
    - priority
    - concurrency limit
- document intended usage (UI, gameplay, ambience)

When adding new features:
- spatial audio (if introduced) must remain optional
- advanced effects (reverb, filters) must degrade gracefully if unsupported

---

## 9. Invariants

- Audio identifiers are stable and versioned implicitly
- Audio playback must never block rendering or input
- Excessive audio spam is actively prevented
- Visual feedback always remains authoritative
