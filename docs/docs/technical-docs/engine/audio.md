---
id: audio
title: Audio
sidebar_label: Audio
---

# Audio System

The audio module manages all sound effects and background music.

## Features

- Play and stop sounds.
- Volume control per entity or global.
- Looping music tracks.
- Positional audio for immersive experience.

## Example

```cpp
AudioSystem::playSound("laser.wav");
AudioSystem::playMusic("background.mp3", loop=true);
