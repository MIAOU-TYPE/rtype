---
title: UDP Channel
sidebar_label: UDP
---

# UDP Channel

The UDP channel implements the **real-time data plane**. It is optimized for
freshness, not reliability: late data is discarded rather than delayed.
This channel carries the bulk of gameplay-related traffic.

---

## Role of UDP

UDP is used when:
- low latency is critical
- updates are frequent
- newer data always supersedes older data

UDP traffic must never drive authoritative decisions on its own.

---

## Typical Use Cases

### Server → Client
- authoritative world snapshots
- entity transforms and states
- time-sensitive gameplay updates

### Client → Server
- player input commands:
    - movement
    - shooting
    - quick actions

### Optional Transient Data
- non-critical notifications
- effects or hints that can be dropped safely

---

## Ordering, Loss, and Duplication

UDP provides **no guarantees**.

You must assume:
- packets may be dropped
- packets may arrive out of order
- packets may be duplicated

### Required countermeasures
- include a monotonically increasing **sequence number**
- drop snapshots older than the last accepted sequence
- ignore duplicates silently

No implicit reliability is assumed.

---

## Reliability Boundaries

- UDP packets are processed at most once
- No retransmission is attempted unless explicitly designed
- Critical state changes must be confirmed via TCP

UDP is strictly best-effort.

---

## Binding and Anti-Spoofing

UDP has no built-in notion of identity. Binding establishes trust.

### Recommended pattern
1. Authenticate over TCP
2. Receive a short-lived **UDP bind token**
3. Client sends `UDP_BIND { token }`
4. Server validates token and associates `(ip, port)` with the session

After binding:
- only packets from the bound endpoint are accepted
- packets from unknown endpoints are dropped
- rebinding is required on NAT change or reconnect

Bind tokens must:
- be opaque
- have a short TTL
- be single-use or explicitly renewable

---

## Client-Side Processing Rules

- UDP receive is non-blocking
- RX buffers are bounded
- On overflow:
    - drop packets
    - surface a “connection degraded” signal
- Parsing happens on the networking side
- Only decoded, validated data is forwarded to the game thread

No gameplay code runs in the UDP networking thread.

---

## Failure Handling

- Temporary packet loss → interpolation absorbs jitter
- Prolonged loss → freeze state and mark degraded connection
- Bind failure → retry bind, then fall back to TCP recovery

---

## Invariants

- UDP data is never trusted before binding
- Newer data always overrides older data
- Packet loss does not block rendering or input
- UDP failure never corrupts authoritative state

---
