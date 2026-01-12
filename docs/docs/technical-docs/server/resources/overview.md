---
title: Overview
---

# Server Resources Overview

This section documents **server-side resources**: configuration, static data,
and persistence inputs required for the server to operate correctly.

Unlike the client, the server does not manage visual or audio assets. Server
resources are concerned with **configuration, rules, and durability**, not
presentation.

---

## What Is a Server Resource

On the server, a resource is any external data that:

- influences runtime behavior
- configures limits or rules
- persists state across restarts
- must be validated before the server starts

Resources are read by the runtime and owned by the server process.

---

## Resource Categories

### Configuration
- network ports (TCP / UDP)
- tick rate and snapshot rate
- room limits (max rooms, max players)
- rate limits (auth attempts, bind attempts)
- feature flags

Configuration is typically loaded from:
- config files (JSON, TOML, YAML)
- environment variables
- command-line arguments

---

### Persistence
- authentication database
- account data
- session or token storage (if persisted)
- server-side progression or stats (if applicable)

Persistence resources must be initialized before accepting clients.

---

### Static Data
- server-side level descriptors
- spawn tables or wave definitions
- balance parameters

These resources are read-only at runtime and must not be mutated.

---

## Loading Strategy

### Startup Phase
At startup, the server must:

1. Load configuration
2. Validate mandatory fields and ranges
3. Initialize persistence backends
4. Fail fast on missing or invalid resources

No network listener should be opened before resources are validated.

---

### Runtime Access
- Resources are accessed through well-defined interfaces
- Subsystems receive references or handles, not raw paths
- Direct filesystem access from gameplay systems is forbidden

---

## Validation Rules

- All required resources must exist at startup
- Configuration values must be range-checked
- Schema or version mismatches must be detected early
- Partial or lazy loading is discouraged on the server

A misconfigured server should refuse to start.

---

## Ownership and Lifetime

- The runtime owns resource instances
- Resources live for the entire process lifetime unless explicitly reloaded
- Reloading (hot-reload) is optional and must be safe and explicit

---

## Error Handling

- Resource loading errors are fatal at startup
- Errors must be logged with clear context
- Runtime must exit cleanly without partial initialization

---

## Invariants

- Server resources are non-visual and non-interactive
- Resources are loaded before networking starts
- No gameplay logic depends on filesystem paths
- Resource state is stable and authoritative
- Invalid configuration must never reach runtime execution
