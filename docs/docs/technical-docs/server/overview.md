---
id: overview
title: Technical Documentation Overview
sidebar_label: Overview
---

# Technical Documentation - R-TYPE

Welcome to the **R-TYPE Technical Documentation**!  
This section is intended for developers and contributors who want to understand the inner workings of the R-TYPE game engine, networking system, and gameplay mechanics.

---

## Project Overview

R-TYPE is a classic horizontal shoot’em up, reimagined as a multiplayer game.  
The goal of this project is to:

- Build a **custom C++ game engine** with modular architecture.
- Implement **real-time multiplayer networking** over UDP.
- Create a faithful recreation of the classic R-TYPE gameplay.

---

## Architecture

The R-TYPE project is organized around three main components:

1. **Engine**
    - Built from scratch in **C++20**.
    - Modular design using **Entity Component System (ECS)**.
    - Handles **rendering, audio, physics, AI**, and game logic.
    - Designed to be reusable and extensible.

2. **Networking**
    - Client-server architecture using **UDP**.
    - Ensures server authority for game state.
    - Handles **reliability**, **latency**, and **synchronization** between clients.
    - Implements messages for player actions, enemy updates, and projectiles.

3. **Gameplay**
    - Levels, waves, and bosses modeled after classic R-TYPE.
    - Weapons, power-ups, and scoring mechanics.
    - Designed for **real-time multiplayer sessions**.

---

## Navigation

Use the sidebar to explore each part of the technical documentation:

- **Engine**: Learn about ECS, rendering pipeline, physics, audio, and AI modules.
- **Networking**: Understand the UDP protocol, message reliability, and server-client synchronization.
- **Gameplay**: Explore levels, enemies, weapons, bosses, and game logic.
