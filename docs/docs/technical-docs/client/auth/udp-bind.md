---
title: UDP Bind
---

# UDP Bind

UDP binding associates a **connectionless UDP endpoint** with an already
**authenticated session**. It is a mandatory safety step before any gameplay
traffic is accepted by the server.

---

## Purpose

Because UDP is connectionless, the server cannot trust the source of a datagram
by default. UDP binding exists to:

- prevent spoofed or injected packets
- ensure snapshots are sent to the correct client endpoint
- associate gameplay traffic with an authenticated identity

No gameplay packet is trusted before a successful bind.

---

## Binding Token

The bind process relies on a **short-lived, server-issued token**:

- delivered during TCP authentication (`AUTH_OK` or welcome phase)
- opaque to the client
- bound to a specific authenticated session
- expires quickly to limit abuse

The token is never reused across accounts.

---

## Recommended Flow

1. Client authenticates over TCP
2. Server provides a `udpBindToken`
3. Client sends `UDP_BIND { udpBindToken }` to the server UDP port
4. Server validates:
   - token exists
   - token is not expired
   - token is unused or explicitly renewable
5. Server records the sender `(ip, port)` as the UDP endpoint for the session
6. Gameplay UDP packets are now accepted and snapshots are sent

---

## Server-Side Rules

- Only one active UDP endpoint per session
- Packets from unknown endpoints are ignored
- Bind tokens must not be guessable
- Bind attempts are rate-limited

If validation fails, the server silently drops the packet.

---

## Client Rebinding Rules

The client must rebind when:

- the local UDP port changes (NAT rebinding)
- snapshots stop arriving for a defined timeout window
- the server explicitly requests a rebind

Rebinding does **not** require full re-authentication unless the token expired.

---

## Failure Handling

If rebinding fails:
- retry binding with backoff
- if repeated failures occur, fall back to TCP re-authentication
- never send gameplay packets while unbound

---

## Invariants

- UDP bind always follows successful authentication
- A UDP endpoint is never trusted implicitly
- Unbound or invalid UDP traffic is ignored
- UDP binding does not block the render or update loops

---
