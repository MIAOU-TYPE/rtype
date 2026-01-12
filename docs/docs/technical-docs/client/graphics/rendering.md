---
title: Rendering Details
---

# Rendering Details

This document captures **engine-specific rendering guidelines**. It is the place
to document assumptions and constraints introduced by the chosen rendering
backend (SFML, SDL, OpenGL, etc.) while keeping higher-level architecture
backend-agnostic.

---

## Sprite and Entity Mapping

### Sprite identification
- Each renderable entity resolves to:
    - a sprite identifier
    - or a region within a sprite atlas
- The mapping is stable and data-driven whenever possible

### Transform derivation
For each frame:
- position is derived from the synchronized world state
- rotation and scale are derived from entity components
- rendering never extrapolates authoritative state on its own

### Animation state
- Animation selection and frame index are derived from:
    - entity state (moving, idle, firing, destroyed)
    - local timers
- Animation logic does not mutate entity state

---

## Render Data Preparation

- World state is converted into render commands before rendering
- Render commands are immutable during the render phase
- Sorting (by layer or Z-order) is performed once per frame

This minimizes backend-specific logic inside the renderer.

---

## Performance Considerations

### Draw call reduction
- Use sprite atlases to batch draw calls
- Minimize texture and state switches per frame
- Group entities by material when possible

### Resource management
- Never load textures or fonts during rendering
- All assets must be preloaded via a resource manager
- Missing assets fall back to a placeholder

### Geometry handling (if applicable)
- Static geometry should use immutable buffers
- Dynamic geometry updates must be minimal and localized

---

## Debug and Instrumentation

Debug tooling must be cheap and optional.

### Recommended toggles
- Bounding boxes / collision shapes
- Network overlay:
    - ping
    - snapshot sequence
    - interpolation delay
- FPS and frame-time counter

### Rules
- Debug overlay
