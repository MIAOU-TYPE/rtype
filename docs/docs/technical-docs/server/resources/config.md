---
title: Config
---

# Configuration

Server configuration defines how the process starts, what limits it enforces, and
how it behaves under load. Configuration must be **validated at startup** and
must never be trusted if it comes from unverified sources.

This document defines recommended config fields, sources, and validation rules.

---

## Sources of Configuration

Configuration may come from multiple sources, applied in priority order:

1. **Command-line arguments** (highest priority)
2. **Environment variables**
3. **Config file** (JSON/TOML/YAML)
4. **Built-in defaults** (lowest priority)

The resolved configuration should be logged once at startup (with secrets masked).

---

## Networking Configuration

### IP and ports
- `bindIp`: default `0.0.0.0` (or `127.0.0.1` for local dev)
- `tcpPort`, `udpPort`: must be valid and not equal unless explicitly allowed

Validation:
- port range: `1..65535`
- IP format validation (IPv4/IPv6 as supported)
- sockets must bind successfully before continuing startup

### Frame limits
- `maxFrameSize` must be bounded (prevents memory abuse on TCP)
- reject any TCP frame larger than this limit

---

## Simulation Configuration

### Tick rate
- `tickRateHz`: fixed simulation rate (commonly 30/60)
- `dt = 1 / tickRateHz`

Validation:
- enforce reasonable bounds (e.g. `10..240`)

### Snapshot rate
- `snapshotRateHz`: frequency of UDP snapshots
- can be equal to or lower than `tickRateHz`

Validation:
- must be `<= tickRateHz`
- enforce bounds (e.g. `5..tickRateHz`)

### Catch-up
- `maxCatchUpTicks`: cap for late frames to avoid death spirals

Validation:
- enforce small upper bound (e.g. `1..8`)

---

## Room Configuration

- `maxRooms`: hard cap on concurrent rooms
- `maxPlayersPerRoom`: enforced on join
- `autoDestroyEmptyRooms`: optional cleanup policy

Validation:
- `maxRooms >= 1`
- `maxPlayersPerRoom` in a sane range (e.g. `1..64`)

---

## Auth and Session Configuration

- `dbPath`: path to auth database (SQLite or equivalent)
- `sessionTtlSec`: session expiration time
- `udpBindTokenTtlSec`: short TTL for bind tokens
- `maxLoginAttempts` and `lockoutWindowSec`: brute-force protection

Validation:
- TTL must be > 0
- bind token TTL should be short (seconds)
- dbPath must be accessible (fail fast)

Security:
- never log raw secrets
- avoid exposing whether a username exists (policy dependent)

---

## Limits and Backpressure Configuration

- `maxClients`: cap to protect the process
- `maxRxPackets`: bounded network buffers (ring buffer sizes)
- `maxTxBytesPerClient`: cap per-connection TX buffering
- `maxInputsQueuedPerClient`: prevent unbounded input spam

Validation:
- all caps must be > 0
- enforce reasonable upper bounds to avoid memory blowups

---

## Logging Configuration

- `level`: debug/info/warn/error
- `json`: structured logs for production
- `file`: optional log file target

Rules:
- log resolved config at startup
- mask credentials, tokens, and secrets

---

## Hot Reload (Optional)

If supported:
- reload only safe fields (logging level, snapshot rate)
- never reload port bindings without a full restart
- reload must be atomic and validated

Hot reload is optional; absence is acceptable.

---

## Startup Checklist

Before opening listeners:
- parse configuration
- apply overrides
- validate all fields and ranges
- validate resource paths (db, data files)
- log sanitized resolved config

If validation fails: exit cleanly.

---

## Invariants

- Configuration is validated before networking starts
- Values are bounded to protect server stability
- Secrets are never logged
- Simulation timing is derived from config only
- Limits prevent unbounded memory growth
