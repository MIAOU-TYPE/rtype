---
title: Assets
---
#  Assets

This document describes how assets are organized, loaded, and managed on the
client. Assets are treated as **data**, not code, and are consumed through
stable identifiers rather than file paths.

---

## Asset Categories

The client typically manages the following asset types:

- Textures and sprite sheets
- Fonts
- Sound effects and music
- Shaders (optional, backend-dependent)
- Data manifests (JSON, TOML, etc.)

Each category is handled by a dedicated loader or resource manager.

---

## Recommended Layout

A clear, predictable directory structure:


assets/
textures/
fonts/
audio/
levels/


Subdirectories may be added per feature, but the top-level layout should remain
stable.

---

## Asset Identification

- Assets are referenced by logical identifiers, not file paths
- Identifiers remain stable even if files are moved or replaced
- Mapping from identifier → file is centralized

This avoids leaking filesystem details into gameplay or UI code.

---

## Loading Strategy

### Startup
- Preload all assets required for menus and UI
- Load lightweight, frequently used resources early

### Room / Game Entry
- Lazy-load heavy gameplay assets when entering a room
- Avoid loading during active gameplay whenever possible

### Caching
- Loaded assets are cached
- Duplicate loads are forbidden
- Assets remain resident until explicitly released or until shutdown

---

## Failure Handling

- Missing or invalid assets must not crash the client
- Load failures should:
  - log a warning
  - fall back to a placeholder asset

The game must remain playable even with degraded assets.

---

## Invariants

- Assets are immutable once loaded
- Asset loading never blocks the render loop
- Resource ownership is centralized
- Filesystem structure is invisible to gameplay logic
