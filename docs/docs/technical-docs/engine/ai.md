---
id: ai
title: AI
sidebar_label: AI
---

# AI System

The AI system manages enemy behaviors and automated game logic.

## Features

- Enemy movement patterns.
- Attack sequences and firing rates.
- Adaptive behaviors based on player position.
- Boss AI with multiple phases.

## Example

```cpp
EnemyAI ai;
ai.setPattern(PATTERN_ZIGZAG);
ai.update(enemy, player);
