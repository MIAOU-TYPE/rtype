---
title: Authentication Flows
---

This document describes the **runtime authentication flows** executed by the client and server.
It focuses on sequencing, state transitions, and error handling rather than packet structure
(which is documented separately in the TCP protocol specification).

All authentication flows are executed over **TCP**.

---

## 1. General Principles

- Authentication is **explicit** and **state-driven**
- The server is the sole authority on identity and session validity
- The client is responsible for:
  - initiating requests
  - reacting to responses
  - updating UI state
  - managing local session data

At no point does authentication logic block the render loop.

---

## 2. Register Flow

### Purpose
Create a new user account and optionally authenticate immediately.

### Sequence

1. User fills:
   - username or email
   - password
2. UI emits `AuthRegisterRequested`
3. Client runtime:
   - allocates a request ID
   - sends `REGISTER` over TCP
4. Server validates:
   - username uniqueness
   - password policy
5. Server responds with:
   - `AUTH_OK` on success  
   - `ERROR_MESSAGE` on failure

### Client behavior on success

Two acceptable strategies (implementation choice):

- **Auto-login**
  - Treat `AUTH_OK` as a login success
  - Populate `AuthContext`
  - Transition directly to authenticated menu/lobby

- **Explicit login**
  - Discard session token
  - Return to login screen
  - Require manual login

Both are protocol-compliant. The current implementation supports auto-login.

### Client behavior on failure

- Display server-provided error message
- Keep user on register screen
- Allow retry without reconnecting TCP

---

## 3. Login Flow

### Purpose
Authenticate an existing user and establish a session.

### Sequence

1. User enters credentials
2. UI emits `AuthLoginRequested`
3. Client runtime:
   - allocates a request ID
   - sends `LOGIN` over TCP
4. Server verifies credentials and account state
5. Server responds with:
   - `AUTH_OK`
   - or `ERROR_MESSAGE`

### Client behavior on success

- Populate `AuthContext`:
  - `userId`
  - `username`
  - `sessionToken`
  - `ttlSec`
- Clear any previous authentication error
- Mark client as authenticated (`authed = true`)
- Trigger UI state transition to authenticated menu

The session token is kept **in memory** and is considered authoritative until:
- expiration
- logout
- explicit invalidation by server

### Client behavior on failure

- Do **not** modify existing token
- Update `AuthContext.authError`
- Notify UI via version increment (`authErrorVersion`)
- Allow retry

---

## 4. Error Handling Semantics

### Credential-related errors
Examples:
- invalid credentials
- user does not exist
- password policy failure

Client action:
- Display error message
- Keep input fields editable
- Allow retry

### Account-related errors
Examples:
- account locked
- banned

Client action:
- Display error message
- Disable repeated retry attempts
- Require user action (wait, support, or restart)

### Internal errors
Examples:
- database failure
- server-side exception

Client action:
- Display generic failure message
- Allow limited retries
- Avoid infinite retry loops

---

## 5. Session Token Handling

### Token acquisition
- Token is received only via `AUTH_OK`
- Token is opaque to the client
- Token lifetime is controlled by the server (`ttlSec`)

### Token usage
- Used implicitly by:
  - TCP session context
  - UDP binding / validation
- Client must not attempt gameplay actions without a valid token

### Token expiration
When the server reports:
- `TOKEN_EXPIRED`
- `TOKEN_INVALID`

Client must:
- Clear in-memory token
- Reset authentication state
- Return to login screen

---

## 6. Persistence Policy (Optional)

Token persistence is **optional** and implementation-defined.

If enabled:
- Token may be stored locally (e.g. config file, OS keychain)
- Token must be:
  - cleared on explicit logout
  - cleared on token-related errors
  - refreshed or discarded on expiration

If disabled:
- Token exists only in memory
- Client must login on each launch

Both models are protocol-compatible.

---

## 7. State Transitions Summary

| Event                 | Resulting State            |
|----------------------|----------------------------|
| App start            | Unauthenticated            |
| REGISTER success     | Authenticated or Login UI  |
| LOGIN success        | Authenticated              |
| AUTH error           | Unauthenticated            |
| TOKEN invalid/expire | Unauthenticated            |
| Logout               | Unauthenticated            |

---

## 8. Invariants

- Authentication requests are serialized by request ID
- Only the latest auth request may update `AuthContext`
- UI state transitions occur on the main thread only
- Authentication failure must never crash the client
- Authentication must not block rendering or input
