---
title: UDP Channel
sidebar_label: UDP
---

# UDP Channel

The UDP channel is the server **data plane** for real-time gameplay. It carries time-sensitive
messages where late delivery is worse than loss. The server uses UDP primarily for:

- receiving client inputs (movement, shoot, quick actions),
- sending authoritative snapshots (world state replication),
- optional transient notifications that can be dropped safely.

UDP is connectionless: packets may be dropped, duplicated, or reordered. The server must be
robust to all three, and must never assume identity from `(ip, port)` without an explicit bind step.

---

# Diagram: UDP Flow

```text
┌───────────────────────────────────────────────────────────┐
│                     APPLICATION SERVER                    │
│                                                           │
│   ┌───────────────┐     ┌──────────────────────────────┐  │
│   │  GameServer   │     │          IGameWorld          │  │
│   │ (IMessageSink)│<--->│     (ECS / game engine)      │  │
│   └──────▲────────┘     └──────────────────────────────┘  │
│          │                            ▲                   │
│          │ network→game events        │ game API          │
│   ┌──────┴────────┐                   │                   │
│   │ IMessageSink  │ interface         │                   │
│   └──────▲────────┘                   │                   │
└──────────│────────────────────────────│───────────────────┘
           │                            │
           │                            │
┌──────────┴────────────────────────────┴───────────────────┐
│                       NETWORK SERVER                      │
│                                                           │
│   ┌─────────────────────────────────────────────────────┐ │
│   │                 ServerRuntime                       │ │
│   │  - starts IServer/UdpServer                         │ │
│   │  - thread 1: runReceiver → readPackets()            │ │
│   │  - thread 2: runProcessor → popPacket()             │ │
│   │                        │                            │ │
│   └───────────────▲────────┴────────────────────────────┘ │
│                   │                                       │
│               IPacket                                     │
│                   │                                       │
│   ┌───────────────┴─────────────┐     ┌─────────────────┐ │
│   │        UDPPacketRouter      │     │ SessionManager   │ │
│   │ - reads PacketHeader        │     │ - sockaddr → id  │ │
│   │ - validates (size/version)  │     │ - id → sockaddr  │ │
│   │ - getOrCreateSession(addr)  │     └─────────────────┘ │
│   │ - parses payload (CONNECT,  │                         │
│   │   INPUT, PING, DISCONNECT)  │                         │
│   │ - calls IMessageSink        │                         │
│   └─────────────────────────────┘                         │
│                                                           │
│   ┌─────────────────────────────────────────────────────┐ │
│   │                  UDP Server (transport)             │ │
│   │  - socket UDP                                       │ │
│   │  - recvfrom() → IPacket + push RX queue             │ │
│   │  - pop TX queue → sendto()                          │ │
│   └─────────────────────────────────────────────────────┘ │
└───────────────────────────────────────────────────────────┘
````

---

## Role and Boundaries

### What belongs to UDP

* client inputs (high frequency, lossy tolerated)
* server snapshots (authoritative replication)
* optional transient updates (non-critical)

### What does NOT belong to UDP

* authentication credentials
* room membership decisions
* any state-changing action that must be confirmed

Anything that must be confirmed goes over TCP.

---

## Transport Rules (Loss / Reordering / Duplication)

The server must assume:

* packets can be dropped
* packets can arrive out of order
* packets can be duplicated

Server-side consequences:

* input messages should carry a `sequence` or timestamp
* the server may drop stale inputs
* duplicate inputs must be tolerated safely (idempotent handling when possible)
* snapshot send never blocks simulation; if a snapshot is dropped, the next one supersedes it

UDP must not implement implicit reliability. If reliability is needed, it must be explicit and documented.

---

## Identity, Binding, and Anti-Spoofing

UDP has no inherent connection identity. The server must not trust `(ip, port)` by default.

### Recommended binding pattern

1. Client authenticates over TCP and receives a session token
2. Server issues a short-lived `udpBindToken` for that session
3. Client sends `UDP_BIND { udpBindToken }` on the UDP port
4. Server validates:

    * token exists
    * token not expired
    * token not already used (unless renewable by design)
5. Server associates the sender `(ip, port)` with the authenticated session

After successful binding:

* accept gameplay UDP packets only from the bound endpoint
* ignore packets from unknown endpoints
* require rebind if NAT changes or on reconnect

### Security requirements

* bind tokens must be opaque and unguessable
* bind attempts must be rate-limited
* bind tokens should have short TTL

---

## Server Runtime Integration

A typical server runtime uses two UDP-side phases:

### 1) Receive phase (network thread)

* `recvfrom()` reads datagrams from the UDP socket
* datagrams are wrapped into `IPacket`/`UDPPacket`
* packets are pushed into a bounded RX queue (ring buffer)

Rules:

* socket must be non-blocking
* RX queue is bounded; on overflow, packets are dropped
* no gameplay logic is executed in the receive thread

### 2) Process phase (processor / simulation thread)

* packets are popped from RX queue
* protocol header is validated (size/version/type)
* packet is routed by type:

    * bind
    * input
    * ping/keepalive
    * disconnect

After decoding:

* validated inputs are enqueued into the owning room’s input queue
* bind updates session endpoint mapping
* disconnect triggers cleanup events

---

## Snapshot Sending (Server → Client)

Snapshots are produced by the simulation layer and sent over UDP.

Rules:

* each snapshot includes a monotonic `sequence`
* snapshot rate can be:

    * every tick, or
    * lower frequency (e.g. 20–30 Hz)
* sending must be best-effort:

    * never block simulation
    * if TX queue is full, drop snapshots rather than grow memory

Snapshots should be addressed using the session’s currently bound `(ip, port)`.

---

## Backpressure and Bounded Queues

UDP itself has no backpressure. The server must enforce bounded memory usage:

* bounded RX ring buffer (drop on overflow)
* bounded per-session or per-room input queues
* bounded TX queues for outgoing snapshots

Under overload:

* drop low-priority traffic first (snapshots over inputs is policy-dependent)
* keep simulation correctness as priority

---

## Failure Modes

### UDP packet loss

* normal behavior; simulation continues
* clients recover via later snapshots

### Prolonged silence from a client

* consider session timeout policy
* may require rebind or disconnect via TCP (design-dependent)

### NAT rebinding

* client endpoint changes `(ip, port)`
* require `UDP_BIND` again
* update session endpoint only after token validation

---

## Invariants

* UDP is time-sensitive and best-effort
* The server never trusts UDP identity without binding
* UDP receive does not execute gameplay logic
* All queues are bounded; drops are acceptable and expected
* Simulation never blocks on UDP IO
* Authoritative decisions are confirmed over TCP
