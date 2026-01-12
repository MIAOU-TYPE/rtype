---
title: Guidelines
---

# Contribution Guidelines

This document defines the **general contribution guidelines** for the R-Type
project. These rules exist to keep the codebase consistent, predictable, and
maintainable as it grows.

All contributors are expected to follow these guidelines.

---

## General Principles

- Prefer **clarity over cleverness**
- Keep behavior **explicit and deterministic**
- Respect the **client/server authority split**
- Write code that is easy to review and reason about
- Do not optimize prematurely

If a change makes the system harder to understand, it is probably wrong.

---

## Code Organization

- Follow the existing directory structure
- Do not introduce new top-level modules without discussion
- Keep responsibilities narrow and well-defined
- Avoid circular dependencies

Client, server, and shared code must remain clearly separated.

---

## Coding Style

### Language rules
- Use modern C++ features where appropriate
- Avoid undefined behavior
- Prefer RAII for resource management
- Avoid raw ownership; use smart pointers consistently

### Readability
- Use descriptive names
- Keep functions short and focused
- Avoid deep nesting
- Prefer explicit types over overly clever templates

### Error handling
- Fail fast on invalid state
- Do not swallow errors silently
- Log errors with clear context

---

## Networking Guidelines

- Never trust client input
- Validate all network payloads
- Enforce size and rate limits
- Never block on network IO in the main loop
- Assume packet loss, duplication, and reordering on UDP

Network code must be defensive by default.

---

## Server Rules

- The server is authoritative
- Simulation runs on fixed ticks
- Do not couple simulation to networking or persistence
- Avoid blocking operations in the simulation loop
- Bound all queues and buffers

---

## Client Rules

- The client is presentation-only
- Never apply authoritative state locally
- Do not block the render thread
- All state changes must be driven by server events

---

## Documentation Rules

- Documentation is part of the codebase
- Any behavior change must update documentation
- Keep docs accurate and minimal
- Prefer explaining *why* over *what*

Outdated documentation is considered a bug.

---

## Testing and Validation

- Build and run locally before submitting a PR
- Test both client and server when applicable
- Verify networking flows (auth, room join, gameplay)
- Avoid committing broken or experimental code

---

## Asset Contributions

- Assets must follow naming conventions
- Avoid breaking existing references
- Keep file sizes reasonable
- Document asset usage when adding new ones

---

## Review Etiquette

- Be constructive and precise
- Critique the code, not the author
- Ask for clarification instead of assuming intent
- Respect review feedback

---

## Invariants

- Server authority is never compromised
- Client never trusts itself
- Networking is defensive
- Code remains readable and maintainable
- Documentation stays in sync with behavior

Following these guidelines ensures the project stays stable and understandable
for everyone involved.
