---
title: Token Persistence
---
# Token Persistence

Persisting authentication tokens can improve user experience (auto-login),
but it introduces additional security considerations. Persistence must remain
**minimal**, **explicit**, and **revocable**.

---

## Purpose

The goal of token persistence is to:
- reduce friction at startup
- avoid unnecessary login prompts
- preserve user intent (“remember me”)

Persistence is optional and must not be required for correct client operation.

---

## What to Store

Only store data that is strictly necessary.

### Allowed
- Session token or refresh token **only if supported by the server**
- A boolean “remember me” flag
- Optional last-used username (for UI convenience only)

All persisted data must be considered sensitive.

---

## What NOT to Store

Never persist:
- Raw passwords
- Password hashes or derived secrets
- Long-lived tokens without:
    - server-side expiration
    - rotation or invalidation support

Storing any of the above is both useless and dangerous.

---

## Storage Options

### Simple local storage
- Configuration file in user-specific directory
- Easy to implement
- Lower security guarantees

### OS-managed secure storage (preferred)
- Keychain / Credential Vault
- Secure enclave when available
- Automatic encryption and access control

The chosen backend must be abstracted behind a small interface to allow
platform-specific implementations.

---

## Invalidation Rules

Persisted tokens must be aggressively invalidated.

### Mandatory invalidation cases
- Explicit logout
- Authentication errors:
    - `TOKEN_INVALID`
    - `TOKEN_EXPIRED`
- User switches account

### Behavior
- Delete token from persistent storage
- Clear in-memory authentication state
- Force return to login screen

No automatic retry with an invalid token is allowed.

---

## Startup Behavior

At application startup:
- If persistence is disabled → show login screen
- If enabled and token exists:
    - attempt session resume (if supported)
    - on failure, delete token and fallback to login

Failures must be silent and non-blocking.

---

## Invariants

- Token persistence is opt-in
- Tokens are never trusted blindly
- Invalid tokens are deleted immediately
- Client remains functional without persistence
