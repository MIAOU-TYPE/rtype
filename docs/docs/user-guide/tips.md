---
title: Tips
---

# Tips

This section collects **practical advice** and common troubleshooting patterns
when running or testing the R-Type project. Most issues fall into networking,
performance, or state synchronization categories.

---

## Connectivity

### Login works but gameplay never updates
This usually means **UDP traffic is not flowing**.

Check the following:
- UDP is not blocked by a firewall or OS security rule
- NAT or router is not dropping UDP packets
- the UDP bind step exists and succeeds on both client and server

If TCP works but UDP does not, the game will appear “frozen” after login.

---

### Local network (LAN) issues
If clients on other machines cannot connect:
- ensure the server bind IP is `0.0.0.0` (or the correct interface)
- avoid binding only to `127.0.0.1` when expecting LAN clients
- confirm that the server machine’s firewall allows inbound TCP and UDP traffic

---

## Performance

### Stutter or jitter
If movement or animations feel unstable:
- confirm snapshot interpolation is enabled
- ensure rendering uses `now - interpolationDelay`
- verify snapshot sequence numbers increase monotonically

Reducing snapshot rate can save bandwidth, but:
- do not reduce it below what gameplay requires
- simulation tick rate and snapshot rate must remain consistent

---

### Local overload
Running:
- the server
- multiple clients
- and heavy debug logging

on the same machine can cause artificial lag.

Recommendations:
- reduce logging verbosity during gameplay
- avoid debug prints in hot paths (network receive, simulation tick)
- test networking on at least two machines when possible

---

## Debugging

### Recommended overlays
If supported by the project, enable:
- FPS counter
- network statistics (ping, packet loss)
- snapshot sequence display
- interpolation delay visualization

These tools make desync and latency issues immediately visible.

---

### Useful logs
At minimum, logs should include:
- TCP connect and disconnect events
- authentication success and failure
- UDP bind success or rejection
- snapshot sequence progression
- room join and leave events

Avoid logging raw packet contents in production.

---

## Authentication

### Testing accounts
- never reuse real passwords
- use simple, disposable credentials during testing
- avoid checking test credentials into repositories or scripts

---

### Token persistence
If the client stores auth tokens:
- expired or invalid tokens must be cleared automatically
- token-related errors should force a return to the login screen
- “logout” should explicitly invalidate and delete stored tokens

Unexpected auth behavior often comes from stale tokens.

---

## Known Failure Patterns

| Symptom | Likely Cause |
|-------|-------------|
| TCP works, UDP silent | Firewall, NAT, or missing UDP bind |
| Login succeeds, stuck in lobby | Missing or unhandled TCP response |
| Room join requested but no transition | UI not reacting to server confirmation |
| Inputs appear ignored | Session not bound to UDP endpoint |
| Entities teleport or freeze | Snapshot ordering or interpolation bug |

---

## General Advice

- Always assume packet loss and reordering
- Never block the render loop on networking
- Prefer failing fast and loudly during development
- Trust the server, not the client’s local state

Most bugs become obvious once you inspect **who owns authority** and **which
channel (TCP vs UDP)** is responsible for the missing signal.
