---
title: Auth UI and Events
---

# Auth UI and Events

This document defines how the UI reflects authentication state changes and how
those changes are driven by network events. The UI is **reactive**: it never
decides authentication outcomes, it only reacts to them.

---

## Authentication UI States

The UI maintains a small, explicit state machine:

- `LoggedOut`  
  Default state. Login and register forms are visible.

- `LoggingIn`  
  An authentication request is in flight. Input is locked.

- `LoggedIn`  
  Authentication succeeded. User has access to lobby/menu features.

- `AuthError`  
  Transient state used to surface an error message to the user.

The UI state is derived from network responses, not from user intent alone.

---

## Event Model

UI actions and network results are expressed as events.

### Request events (UI → runtime)
- `Auth.LoginRequested(username, password)`
- `Auth.RegisterRequested(username, password)`
- `Auth.LogoutRequested()`

### Result events (runtime → UI)
- `Auth.LoginSucceeded(sessionToken)`
- `Auth.LoginFailed(code, message)`
- `Auth.LogoutSucceeded()`
- `Network.Disconnected(channel)`

This separation ensures the UI remains testable and deterministic.

---

## State Transitions

### Login
1. User submits credentials  
   → `LoggedOut` → `LoggingIn`
2. Runtime sends `LOGIN`
3. Server response:
  - success → `LoggedIn`
  - failure → `AuthError` → `LoggedOut`

### Logout
1. User requests logout  
   → `LoggedIn` → `LoggingIn`
2. Runtime clears session
3. UI returns to `LoggedOut`

### Network failure
- `Network.Disconnected(TCP)` while authenticated  
  → clear session → `LoggedOut`

---

## UI Behavior Rules

- Submit buttons are disabled while in `LoggingIn`
- Duplicate auth requests are ignored
- Error messages are displayed close to the relevant field when possible
- Errors are cleared automatically on next user input

---

## Post-Login Actions

On successful login:
- Switch to lobby or authenticated menu
- Trigger UDP bind if required by the networking design
- Enable features gated by authentication state

The UI must not assume UDP bind success; networking failures are handled
independently.

---

## Invariants

- UI state always reflects network truth
- Authentication errors are non-fatal
- UI never blocks on network operations
- Logout always clears local authentication state
