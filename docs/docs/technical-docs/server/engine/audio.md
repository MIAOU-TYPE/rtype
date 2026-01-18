---
id: audio
title: Audio
sidebar_label: Audio
---

# Audio System

The audio system manages sound effects and music on the server side. The server does not play audio directly - it tracks audio events and includes them in snapshots sent to clients.

## Features

- Sound effect triggering for game events.
- Music track management.
- Audio state synchronization across clients.
- Volume and priority management.

## Components Used

- **Attack**: May include sound effect references
- **Projectile**: Sound effects for firing
- **Health**: Death sound effects
- **PowerUp**: Collection and activation sounds

## Workflow

1. Track audio events during gameplay (shots, deaths, power-ups).
2. Include audio commands in network snapshots.
3. Ensure all clients play synchronized sound effects.
4. Manage music transitions for level changes.

## Server Audio Responsibilities

- Determine when sounds should play
- Include audio events in authoritative snapshots
- Ensure consistent audio experience across all clients
- Handle music track changes for game phases

Actual audio playback is handled by clients using SFML audio capabilities.