---
title: Audio Overview
---

# Overview

The audio subsystem provides **sound effects** and **music playback** synchronized with visual
and UI events. It is a presentation-layer system: it reacts to game and UI signals but never
drives gameplay or networking logic.

---

## Responsibilities

The audio module is responsible for:

- Loading and managing audio assets (SFX and music)
- Playing short, one-shot sound effects:
    - shooting
    - hits
    - explosions
    - UI feedback
- Playing and stopping looping music tracks:
    - menu
    - lobby
    - in-game
- Applying global audio rules:
    - volume groups
    - priorities
    - concurrency limits

The audio subsystem must remain optional: the client must function correctly even if audio
initialization fails.

---

## Integration Model

### Event-driven interaction

Other systems **do not** call the audio backend directly.

Instead:
- Systems emit semantic audio events  
  Example: `Audio.PlaySfx("sfx.shoot")`
- The audio module consumes these events and resolves:
    - which asset to play
    - how loudly
    - with which priority
    - whether it can be played (concurrency limits)

This keeps audio logic decoupled from gameplay and UI implementation details.

---

## Scene-Based Music Control

- Only one music track is active at a time
- Scene transitions (menu → lobby → game) trigger music changes
- Music transitions must be:
    - explicit
    - non-blocking
    - optionally cross-faded (if supported)

---

## Decoupling Guarantees

- Audio has no knowledge of:
    - networking state
    - server authority
    - protocol details
- Network events may indirectly cause audio playback through UI or world events, but audio
  never reacts to network packets directly.

---

## Invariants

- Audio playback must never block rendering or input
- Missing audio assets must not crash the client
- Audio events are best-effort and disposable
- Visual feedback remains authoritative

---
