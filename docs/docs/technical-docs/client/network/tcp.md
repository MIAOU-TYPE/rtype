---
title: TCP Channel
sidebar_label: TCP
---

# TCP Channel

The TCP channel implements the **control plane** of the client. It is reserved
for low-frequency, high-importance exchanges that require reliability,
ordering, and explicit confirmation.

---

## Role of TCP

TCP is used whenever correctness is more important than latency.

Typical responsibilities include:
- authentication and session lifecycle
- account and profile queries
- room discovery and room lifecycle
- game start / end signaling
- delivery of critical errors

TCP traffic is sparse and stateful.

---

## Typical Use Cases

- Login / Register / Logout
- Fetch user or profile metadata
- Room listing, creation, and deletion
- Join / leave room confirmation
- Server-side validation errors

All state transitions driven by TCP are authoritative.

---

## Request / Response Pattern

All TCP interactions follow a strict request/response model.

### Client behavior
- Attach a monotonically increasing **request ID** to each request
- Send request asynchronously
- Never wait synchronously for a response

### Server behavior
- Respond with the same request ID
- Return either:
    - a success payload
    - an error payload

### Client handling
- Match responses by request ID
- Update local state
- Emit high-level events for UI or runtime

This pattern keeps the render loop non-blocking and reactive.

---

## Error Semantics

- Errors are explicit TCP messages
- Errors always reference a request
- Unknown or mismatched responses are treated as protocol violations

On protocol violation:
- log the error
- disconnect immediately
- reset client state

Fail-fast behavior prevents undefined states.

---

## Keep-Alive Strategy

If required by the server:
- send periodic keep-alive messages
- track last received response timestamp
- enforce a timeout window

If no response is received within the timeout:
- close TCP connection
- treat it as a hard failure

---

## Disconnect Handling

A TCP disconnect implies loss of authoritative state.

Client must:
- clear authentication and session data
- stop gameplay-related activity
- return to main menu

UDP traffic must be ignored once TCP is disconnected.

---

## Client-Side Rules

- TCP must never block the render thread
- All outgoing messages are serialized through a single send queue
- Network IO is isolated in the TCP thread
- Protocol mismatches always trigger a disconnect

---

## Invariants

- TCP defines authoritative control state
- Every request has at most one response
- UI never waits synchronously on TCP
- TCP failure always results in a full state reset

---
