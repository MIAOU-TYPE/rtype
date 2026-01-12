---
title: Authentication Overview
---

Authentication is responsible for:

- Identifying the user (login/register)
- Exchanging credentials securely over TCP
- Obtaining a **session token** used by the server to link TCP ↔ UDP
- Persisting session data client-side (optional) to improve UX

## Components

- `flows.md`: end-to-end scenarios (register, login, logout, reconnect)
- `tcp-protocol.md`: message contracts and errors
- `udp-bind.md`: binding rules once authenticated
- `persistence.md`: where/how you store tokens (and what not to store)
- `ui-events.md`: how UI reacts to auth state changes

## Security boundaries (client side)

- The client never stores raw passwords.
- The client treats tokens as **secrets**:
  - never log tokens in production logs
  - clear tokens on logout
  - clear tokens on auth failure
