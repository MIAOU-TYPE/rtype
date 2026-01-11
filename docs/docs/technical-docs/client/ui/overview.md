---
title: UI Overview
---

# UI Overview

The UI layer is responsible for **presentation and user interaction**. It
renders screens, collects user intent, and displays feedback, while remaining
fully decoupled from networking and authoritative gameplay logic.

---

## Responsibilities

The UI layer provides:

- Core screens:
    - main menu
    - login / register
    - lobby
    - settings
    - in-game HUD
- Input handling for UI interactions
- Translation of user actions into **intent events** for the runtime
- Visual feedback for:
    - authentication status
    - connection state
    - errors and warnings

The UI never decides game outcomes.

---

## Integration Model

- The UI emits events expressing intent
- The runtime interprets these events and interacts with networking
- The UI observes runtime state through a **read-only view model**

This keeps responsibilities clear and testable.

---

## View Model Access

- UI reads a minimal, structured view of client state
- The view model exposes:
    - current screen mode
    - authentication status
    - room and lobby metadata
    - connection health indicators

The UI must not access world or networking internals directly.

---

## Rules

- UI must not parse or interpret network packets
- UI must not own authoritative gameplay state
- UI logic must remain deterministic and side-effect free
- All network-dependent feedback is driven by runtime events

---

## Error Handling

- Errors are displayed in context (forms, banners, overlays)
- Errors never block navigation permanently
- UI always remains responsive, even during network failure

---

## Invariants

- UI state reflects runtime truth
- UI emits intent, never commands
- UI remains functional without network connectivity
- Presentation logic never leaks into gameplay or networking

