---
title: Resources Overview
---

# Resources Overview

Resources cover the **loading, validation, and lifetime management** of all
external data consumed by the client, including assets and level descriptions.
They form a dedicated infrastructure layer, separate from gameplay and UI.

---

## Responsibilities

The resource system is responsible for:

- Defining where resources live on disk
- Mapping stable identifiers to concrete files
- Loading and caching resources
- Returning lightweight handles:
    - texture IDs
    - sound IDs
    - font IDs
    - level IDs
- Validating required resources at startup

Resource lookup failures must be detected early and explicitly.

---

## Resource Identification

- Resources are referenced by logical identifiers
- Identifiers are stable and backend-agnostic
- File paths are resolved internally by the resource system

No other system is allowed to manipulate file paths directly.

---

## Loading and Caching

- Each resource is loaded at most once
- Loaded resources are cached and reused
- Handles are cheap to copy and safe to store
- Resource unloading is explicit and controlled

---

## Startup Validation

At startup:
- Validate presence of all mandatory resources
- Fail fast on missing critical assets
- Optional or cosmetic resources may fall back to placeholders

This prevents undefined behavior later in runtime.

---

## Layout Rules

- Never hardcode absolute paths
- Use a predictable directory layout:
    - `assets/`
    - `levels/`
    - optional manifests for grouping

Filesystem structure must remain transparent to gameplay logic.

---

## Extensibility Guidelines

- New resource types must integrate through the same loader interface
- Resource formats should be versioned
- Platform-specific handling must remain encapsulated

---

## Invariants

- Resource ownership is centralized
- Resource loading never blocks the render loop
- Resource identifiers outlive file paths
- Missing resources are handled deterministically

