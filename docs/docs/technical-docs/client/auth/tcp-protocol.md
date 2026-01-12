---
title: TCP Authentication Protocol
---

# TCP Authentication Protocol

This document specifies the TCP-based authentication protocol used by the R-Type client and server.
It is written to allow a third-party client implementation to authenticate correctly against the server.

---

## 1. Scope and Rationale

Authentication is a **stateful, reliable** process that involves credentials, identity, and session lifecycle.
TCP is used because:

- Credentials must never be retransmitted implicitly (as with UDP retries).
- Errors must be deterministic and correlated to a specific request.
- Authentication is low-frequency and not latency-critical.

Once authentication succeeds, the client may bind its authenticated identity to the UDP channel.

---

## 2. Connection Lifecycle (Client Perspective)

1. Establish TCP connection to server
2. Perform protocol handshake (`HELLO`)
3. Send authentication request (`REGISTER` or `LOGIN`)
4. Receive either:
    - `AUTH_OK` → authenticated session established
    - `ERROR_MESSAGE` → authentication failed
5. Optionally resume or refresh session using token (future extension)
6. Proceed with lobby / room flow over TCP
7. Gameplay continues over UDP

---

## 3. Message Model

All messages follow the global TCP framing format:

```

[uint32 frame_size][TCP payload]

```

Each payload contains a TCP header followed by a message-specific body.

---

## 4. Authentication Messages

### 4.1 REGISTER

Creates a new user account.

**Direction**: Client → Server  
**Reliability**: Required  
**Idempotency**: No

Payload fields:
- `username` (str16)
- `password` (str16)

Rules:
- Username must be unique
- Password must satisfy server-side policy
- Password is never stored or logged in clear form

---

### 4.2 LOGIN

Authenticates an existing user.

**Direction**: Client → Server  
**Reliability**: Required

Payload fields:
- `username` (str16)
- `password` (str16)

Rules:
- Credentials are validated server-side
- Failed attempts may trigger rate limiting or account lock

---

### 4.3 LOGOUT (future extension)

Explicitly invalidates a session token.

**Direction**: Client → Server  
Payload fields:
- `sessionToken` (u64)

---

### 4.4 RESUME_SESSION (optional / future)

Attempts to resume a previous session without credentials.

**Direction**: Client → Server  
Payload fields:
- `sessionToken` (u64)

---

## 5. Server Responses

### 5.1 AUTH_OK

Returned on successful authentication.

**Direction**: Server → Client

Payload fields:
- `userId` (u32)
- `username` (str16)
- `tokenHi` (u32)
- `tokenLo` (u32)
- `ttlSec` (u32)

Notes:
- `tokenHi` and `tokenLo` form a 64-bit session token
- Token is required for:
  - Session persistence
  - UDP binding
  - Privileged actions
- `ttlSec` defines token expiration time (server authority)

---

### 5.2 ERROR_MESSAGE

Returned on authentication or protocol failure.

**Direction**: Server → Client

Payload fields:
- `code` (u16)
- `message` (str16)

Errors are always correlated with a request ID.

---

## 6. Error Codes

The following error codes are currently defined:

- `INVALID_CREDENTIALS`
- `USER_ALREADY_EXISTS`
- `PASSWORD_POLICY_FAILED`
- `ACCOUNT_LOCKED`
- `TOKEN_EXPIRED`
- `TOKEN_INVALID`
- `INTERNAL_ERROR`

The server may extend this list. Clients must treat unknown codes as fatal authentication failures.

---

## 7. Client Error Handling Rules

On `ERROR_MESSAGE`:

### Credential-related errors
- Display user-friendly message
- Allow retry with corrected input

### Token-related errors
- Clear persisted token
- Force return to login state

### Internal or protocol errors
- Allow limited retry
- Avoid infinite retry loops
- If persistent, abort connection

---

## 8. UDP Binding After Authentication

After `AUTH_OK`:

- The TCP session token is used to authenticate the UDP channel
- UDP packets sent before authentication are ignored by the server
- The server may reject UDP traffic if:
  - Token is missing
  - Token is expired
  - Token does not match TCP session

---

## 9. Security Considerations

- Credentials are **never** sent over UDP
- TCP must respect the project’s security baseline:
  - No clear-text credentials on hostile networks
  - TLS or equivalent transport security if available
- Tokens are opaque and server-generated
- Client must treat tokens as sensitive secrets

---

## 10. Implementation Mapping (Current Code)

| Concept              | Client Component            |
|---------------------|-----------------------------|
| Auth request        | `TCPPacketFactory`          |
| Auth response       | `TCPPacketRouter::onAuthOk` |
| Error handling      | `onErrorSubscribe`          |
| Session storage     | `Engine::AuthContext`       |
| State transition    | `ClientRuntime::runDisplay` |

---

## 11. Extensibility Notes

The protocol is designed to allow future extensions without breaking compatibility:

- Multi-factor authentication
- Session refresh
- Account recovery
- Admin or moderator roles

All extensions must preserve:
- Backward compatibility
- Explicit error signaling
- Server authority
