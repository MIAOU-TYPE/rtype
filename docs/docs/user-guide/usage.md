---
title: Usage
---

# Usage

This page describes the **typical runtime flow** of the R-Type project from a user
perspective. It focuses on what a user or tester does at runtime and how the
system behaves in response.

The details of protocols and architecture are intentionally abstracted here.

---

## Server Usage

The server is designed to run **continuously** and independently of any client.

### Starting the Server

1. Launch the server executable
2. Verify that it starts without configuration errors
3. Confirm that it is listening on both:
    - a TCP port (control plane)
    - a UDP port (gameplay data)

Once started, the server:
- accepts incoming connections
- manages rooms and sessions
- runs the simulation continuously

The server should remain running while clients connect, play, and disconnect.

---

### Expected Server Behavior

- **TCP** is used for:
    - authentication
    - room creation and joins
    - critical confirmations and errors

- **UDP** is used for:
    - player inputs
    - authoritative snapshots

The server:
- remains authoritative at all times
- continues simulation even if some clients disconnect
- does not depend on client frame rate or timing

---

## Client Usage

### First Launch

1. Launch the client application
2. Navigate to the **Login** or **Register** screen
3. Authenticate using valid credentials

On successful authentication:
- a session is established
- the client enters the lobby

---

### Lobby Flow

In the lobby, the client can:

- list available rooms
- create a new room
    - choose a room name
    - choose a maximum player count
- join an existing room (if capacity allows)

All lobby actions are:
- sent to the server over TCP
- confirmed by the server before the UI transitions

---

### In-Room and Gameplay Flow

Once inside a room:

1. The client waits for the server to start the game
2. When the game starts:
    - the server transitions the room state
    - the client switches to in-game mode

During gameplay:
- player inputs are sent to the server over UDP
- the server runs the authoritative simulation
- the client receives periodic UDP snapshots
- rendering uses interpolation for smooth motion

The client never assumes authority over game state.

---

### Leaving, Errors, and Disconnects

- Leaving a room:
    - requires server confirmation
    - returns the client to the lobby

- TCP disconnect:
    - clears session state
    - returns the client to the main menu
    - may display an error message

- Auth token errors:
    - invalidate the current session
    - redirect the client back to login
    - clear any persisted tokens

---

## Common User Actions

- **Create a room**  
  Lobby → Create Room → Confirm → Join

- **Join a room**  
  Lobby → Select Room → Confirm

- **Start a game**  
  Controlled by server or room owner (implementation-specific)

- **Quit the client**  
  Client exits; server continues running

---

## Notes

- UI transitions always follow **server confirmation**
- Gameplay does not start until explicitly triggered by the server
- Network failures are surfaced as UI state changes
- The client is designed to fail safely back to menu or login
