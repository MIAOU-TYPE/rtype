---
title: State Synchronization
sidebar_label: State Sync
---

# State Synchronization

State synchronization converts **authoritative UDP snapshots** into a smooth,
renderable client-side state. It is responsible for absorbing network
irregularities while keeping visual output stable and deterministic.

---

## Role and Constraints

- Server remains authoritative at all times
- Client never simulates missing authoritative state
- Late data is discarded rather than replayed
- Rendering must remain independent of network jitter

State sync operates strictly between networking and rendering.

---

## Snapshot Buffer

The client maintains a **small, ordered snapshot buffer** (typically 2–5
snapshots).

### Buffer rules
- Insert snapshots ordered by `sequence`
- Drop duplicates
- Drop snapshots older than the last rendered baseline
- Keep buffer size bounded

This ensures out-of-order delivery does not destabilize interpolation.

---

## Rendering Time and Interpolation

To smooth motion:
- Rendering is performed at `now - interpolationDelay`
- Typical delay range: **50–150 ms**, configurable

For each frame:
1. Compute render timestamp
2. Select two snapshots that surround that time
3. Interpolate entity transforms between them

If only one snapshot is available:
- Render the latest snapshot
- Accept limited jitter rather than extrapolating incorrectly

---

## Applying Snapshots

Snapshots are applied using a strict, deterministic mapping.

### Entity lifecycle rules
- `entityId` not present locally → create entity
- `entityId` missing from snapshot → destroy or mark inactive
- Existing entity → update components

### Updated components
- transform (position, rotation, scale)
- visual state (sprite, animation state)
- render-relevant flags only

Gameplay logic is never updated here.

---

## Determinism Guarantees

- Snapshot application does not depend on frame rate
- Interpolation factor is derived from timestamps, not frame delta
- Given the same snapshot stream, the visual result is identical

---

## Edge Case Handling

### Long packet loss
- Freeze on last valid state
- Mark connection as degraded
- Resume normally when snapshots resume

### Large time jump
- Snap immediately to newest snapshot
- Clear interpolation history

### Room or session switch
- Clear entity registry
- Clear snapshot buffer
- Reset interpolation state

---

## Invariants

- Server state is the only source of truth
- Client never extrapolates authoritative data blindly
- Out-of-order packets do not affect correctness
- Visual smoothness never compromises determinism

---
