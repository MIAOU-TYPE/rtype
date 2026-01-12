---
id: auth-sessions-udp
title: Sessions, Identity TTL, and UDP Token
sidebar_label: Sessions & UDP Token
---

# Sessions, Identity TTL, and UDP Token (server)

Authentication is tied to the **server session** managed by `Net::Server::SessionManager`.

This document explains:

- how a TCP connection maps to a `sessionId`
- how an authenticated identity is attached to a session with a TTL
- how the UDP token is used to safely bind a UDP address to that session

## Session creation (TCP)

Every time the server receives a TCP packet, it calls:

- `SessionManager::getOrCreateSession(sockaddr_in)`

This assigns an integer `sessionId` and stores the TCP address->session mapping.

## Identity binding + TTL

When `AUTH_REGISTER` or `AUTH_LOGIN` succeeds, the server calls:

- `SessionManager::setIdentity(sessionId, Auth::Identity{userId, username}, ttl)`

Current TTL:

- `ttlSec = 24 * 60 * 60` (1 day)

### Expiration behavior

`SessionManager::isAuthed(sessionId)` returns false when:

- the identity is missing, or
- the current time is beyond the stored expiry timestamp

The TCP router enforces this by rejecting non-auth-free packets with `AUTH_REQUIRED`.

## UDP token generation

A 64-bit token is attached to the session to prevent “UDP address spoofing”.

Token generation happens in two places:

- on `HELLO` -> `WELCOME`
- on successful auth (`AUTH_OK`) if a token does not already exist

The token is generated using `std::random_device` combined with `sessionId`.

## Binding UDP to a session (CONNECT)

UDP traffic starts unbound.

The client must send a UDP `CONNECT` packet containing:

- `sessionId` (u32)
- `tokenHi` (u32)
- `tokenLo` (u32)

Struct:

- `shared/Network/Data/UDP/ConnectData.hpp`

Server-side verification is performed in:

- `server/src/packet/PacketRouter/UDP/UDPPacketRouter.cpp`

### Accept criteria

A `CONNECT` is accepted if:

1. payload size is exactly `sizeof(ConnectData)`
2. the token matches `SessionManager::getUdpToken(sessionId)`
3. `SessionManager::bindUdp(sessionId, fromAddress)` succeeds

If the token does not match, the server logs `Dropped CONNECT: bad token` and ignores the packet.

### Why `bindUdp` can fail

`bindUdp` returns false if the session is unknown on the TCP side (no TCP address mapped for this `sessionId`).

This is deliberate: it forces clients to establish the TCP session first.

## What this means for security

- A client cannot claim a `sessionId` over UDP unless it also knows the 64-bit token that the server generated for that session.
- Replay protection for UDP gameplay packets is handled separately via per-session sequence tracking (`isSequenceValid`).
