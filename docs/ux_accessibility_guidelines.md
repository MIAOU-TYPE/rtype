# UX & Accessibility Guidelines

> **Version:** 1.0  
> **Last Updated:** December 1, 2025  
> **Status:** Draft for Review

## Table of Contents

1. [Introduction](#introduction)
2. [Player Experience (Feel)](#player-experience-feel)
3. [Readability & UI/HUD](#readability--uihud)
4. [Accessibility Requirements](#accessibility-requirements)
5. [Measurable Acceptance Criteria](#measurable-acceptance-criteria)
6. [Developer Checklist](#developer-checklist)
7. [References](#references)

---

## Introduction

This document formalizes the user experience, interface design, and accessibility standards for the R-Type project. It serves as the single source of truth for designers, developers, and QA teams to ensure consistency and quality across all aspects of player interaction.

### Purpose

- Define clear expectations for player sensations and game feel
- Establish UI/HUD standards for readability and clarity
- Ensure the game is accessible to players with diverse needs
- Provide measurable criteria for validation and testing

### Scope

These guidelines apply to:
- Client application UI and HUD
- In-game visual and audio feedback
- Input systems (keyboard)
- Settings and configuration interfaces

<div style="page-break-before: always;"></div>

---

## Player Experience (Feel)

### Overview

The R-Type experience should evoke the classic feel of arcade shooters while retaining a touch of modernity. Players should feel a balance between power, precision, tension, and reward.

### Intended Player Sensations

#### 1. **Power & Agency**
- **Feeling:** Players should feel in control of a powerful spacecraft capable of devastating attacks
- **Implementation:**
  - Weapons must feel impactful with satisfying visual and audio feedback
  - Player ship movement must be precise and responsive

#### 2. **Tension & Challenge**
- **Feeling:** Constant awareness of danger with manageable difficulty
- **Implementation:**
  - Boss enemies must telegraph attacks clearly before execution
  - Difficulty should ramp gradually, not spike unexpectedly
  - Near-misses should be clearly distinguishable from hits

#### 3. **Clarity & Readability**
- **Feeling:** Players always understand what's happening on screen
- **Implementation:**
  - Player ship, projectiles, and enemies must remain visually distinct
  - Critical threats must stand out from background elements
  - UI must never obscure gameplay-critical information

#### 4. **Flow State**
- **Feeling:** Immersion in the "zone" where skill meets challenge
- **Implementation:**
  - Pacing: 10-15 seconds of calm between intense sections
  - Pattern recognition should be rewarded (enemy behaviors, waves)
  - Progression feels earned, not random

<div style="page-break-before: always;"></div>

### Audiovisual Feedback Rules

#### Player Actions

| Action | Visual Feedback | Audio Feedback |
|--------|----------------|----------------|
| **Shoot** | Muzzle flash, projectile trail | Crisp "pew" sound |
| **Hit Enemy** | Impact flash on enemy, particle burst | Satisfying "hit" sound, pitch varies by enemy |
| **Destroy Enemy** | Explosion animation | Explosion sound with bass |
| **Take Damage** | Red flash on ship | Impact sound, damage alarm |
| **Death** | Large explosion, fragments, fade to black | Dramatic explosion, music fade |
| **Power-up Collect** | Glow effect, UI update animation | Positive sound |

#### Boss Enemy Threat Telegraphing

- Telegraph duration: 1000-1500ms
- Visual Cues: Flashing warning + charge anim
- Audio Cues: Loud charge + warning beep

**Implementation Requirements:**
- Telegraph effects must be distinct from background
- Color coding: Orange = warning, Red = imminent danger
- Animations must clearly indicate attack direction
- Audio warning must be audible above music

<div style="page-break-before: always;"></div>

### Difficulty & Progression

#### Flow State Support

1. **Pattern Design**
   - Enemy waves should follow recognizable patterns
   - First encounter of a pattern should be easier (learning phase)
   - Patterns should repeat with variations to reward mastery

2. **Pacing Structure**
   ```
   Easy Wave (15s) → Medium Wave (20s) → Calm (10s) → Hard Wave (25s) → Boss
   ```

3. **Reward Timing**
   - Power-ups should appear after difficult sections
   - Score multipliers should reward continuous play without death
   - Visual rewards (particles, screen effects) for combos

4. **Difficulty Curve**
   - First 3 levels: Learning mechanics
   - Levels 4-6: Mastery and challenge
   - Level 7+: Expert play with all mechanics combined

---

<div style="page-break-before: always;"></div>

## Readability & UI/HUD

### Visual Hierarchy

#### Priority Levels

**Level 1 (Critical - Always Visible):**
- Player health/shield
- Lives remaining
- Active weapon/power-up status
- Warning indicators (low health, boss alerts)

**Level 2 (Important - Visible but Non-Intrusive):**
- Score
- Current level/stage
- Multiplier counter

**Level 3 (Secondary - Can Be Minimized):**
- Settings/pause button
- Network status (multiplayer)
- FPS counter (debug mode)

### Typography Rules

#### Minimum Sizes
- **Critical Info (HP, Lives, Shield):** 16px minimum (default), scales with HUD
- **Standard UI Text (Score, Level, Multiplier):** 14px minimum
- **Labels/Captions:** 12px minimum

#### Font Requirements
- **Primary Font:** High-contrast, sans-serif (e.g., "Roboto", "Inter", "Arial")
- **Accent Font (Titles):** May be stylized but must remain legible
- **Spacing:** 
  - Line height: 1.4-1.6x font size
  - Letter spacing: 0-5% of font size for readability
  
#### Readability Standards
- Text must pass WCAG AA contrast requirements (4.5:1 for normal text)
- Critical text should pass WCAG AAA (7:1) when possible
- Text must be readable on any background (use drop shadows, outlines, or semi-transparent backing)

<div style="page-break-before: always;"></div>

### HUD Layout

#### Fixed Areas

First proposed layout:
```
┌─────────────────────────────────────────────┐
│  [HP]           SCORE: XXXXX HI- XXXXX [Lvl]│  ← Top Bar: Critical Info
├─────────────────────────────────────────────┤
│                                             │
│                                             │
│                                             │
│             MAIN PLAY AREA                  │  ← No HUD Overlap
│                                             │
│                                             │
│                                             │
├─────────────────────────────────────────────┤
│  [Weapon Icon]  [Power-ups]                 │  ← Bottom Bar: Status
└─────────────────────────────────────────────┘
```

Second proposed layout (alternative):
```
┌─────────────────────────────────────────────┐
│                                             │
│                                             │
│                                             │
│                                             │
│             MAIN PLAY AREA                  │  ← No HUD Overlap
│                                             │
│                                             │
│                                             │
├─────────────────────────────────────────────┤
│  [Lvl][Power-ups]    SCORE: XX HI- XX       │  ← Bottom Bar: Status
│  [------HP------]                           │
└─────────────────────────────────────────────┘
```

#### Layout Rules
1. **No Overlap:** HUD elements must NOT overlap main play area (15% top/bottom margins)
2. **Safe Zones:** Critical HUD must stay within 90% of screen (accommodate TV overscan)
3. **Symmetry:** Balance left/right elements for aesthetic coherence
4. **Anchoring:** UI elements must be anchored (not floating) for predictability

<div style="page-break-before: always;"></div>


## Accessibility Requirements

### Overview

The game must be playable by the widest possible audience, including players with visual, auditory, motor, and cognitive disabilities.

### Input Accessibility

#### 1. **Full Remapping**

**Requirement:** Every game action must be remappable.

- **Keyboard:** All keys must be rebindable
- **Mouse:** Optional support, but not required for core gameplay

**Default Layouts:**

*Keyboard (WASD):*
```
Movement: WASD (or ZQSD for AZERTY keyboards) or Arrow Keys
Shoot: Space or Left Ctrl
Special: E or Right Shift
Pause: Esc or P
```

**Implementation:**
- Settings menu must show current bindings
- Rebinding must detect conflicts and warn user
- All bindings must persist between sessions

#### 2. **Input Assistance**

- **Toggle Autofire:** Hold button vs. toggle option for shooting
- **Input Buffering:** Accept inputs 50ms before action is available (reduce timing frustration)

<div style="page-break-before: always;"></div>

### Visual Accessibility

#### 1. **HUD Scaling**

**Requirement:** All UI elements must scale uniformly.

- **Range:** 50% - 200% of default size
- **Increments:** 25% steps (50%, 75%, 100%, 125%, 150%, 175%, 200%)
- **Preserve Layout:** Scaling must not break layout or cause overlaps

#### 2. **Colorblind Modes**

**Requirement:** Provide at least 3 colorblind presets.

| Mode | Target Condition | Implementation |
|------|------------------|----------------|
| **Deuteranopia** | Red-green (green weak) | Replace green → blue/cyan, red → magenta/orange |
| **Protanopia** | Red-green (red weak) | Replace red → dark yellow/brown, green → teal |
| **Tritanopia** | Blue-yellow | Replace blue → red/pink, yellow → cyan/white |

**Elements That Must Remain Distinguishable:**
- Player vs. enemy projectiles
- Health (low vs. full)
- Power-up types
- Enemy types/threat levels
- Team colors (multiplayer)

**Validation:** Use colorblind simulation tools (e.g., Coblis, Color Oracle) to verify.

#### 3. **High Contrast Mode**

- Increase contrast of all UI elements
- Thicker outlines on player/enemies/projectiles (2-3px)
- Disable or reduce particle effects
- Increase brightness of telegraphed attacks

#### 4. **Epilepsy & Flashing Effects**

**Requirement:** Implement flashing reduction options.

- **Warning:** Display epilepsy warning on first launch

<div style="page-break-before: always;"></div>

### Audio Accessibility

#### 1. **Subtitles/Captions**

**Requirement:** All dialogue and critical sound cues must have text equivalents.

- **Sound Cues:** Text indicators for off-screen events ("Explosion [left]", "Enemy approaching [behind]")
- **Formatting:** 
  - Subtitle size: Scalable (same as HUD scaling)
  - Background: Semi-transparent black box for readability
  - Color coding: Optional (e.g., ally=blue, enemy=red)

#### 2. **Audio Mixing**

- **Independent Volume Controls:**
  - Master volume
  - Music volume
  - SFX volume
- **Range:** 0-100% in 5% increments + Mute option

#### 3. **Visual Sound Indicators**

- **On-Screen Indicators:** Optional visual cues for important sounds (explosions, enemy spawns, warnings)
- **Directional:** Arrows or icons indicating sound source direction

### Motor Accessibility

#### 1. **Difficulty Settings**

- **Multiple Difficulty Levels:** Easy, Normal, Hard, Expert
- **Adjustable Parameters:**
  - Enemy health
  - Enemy speed
  - Player invulnerability duration after damage
  - Number of lives

#### 2. **Pause/Slow-Motion**

- **Pause Anytime:** Allow pausing during gameplay (single-player)

### Cognitive Accessibility

#### 1. **Clear Tutorial**

- **Progressive Disclosure:** Introduce mechanics one at a time
- **Practice Mode:** Safe environment to learn without pressure

<div style="page-break-before: always;"></div>

## Measurable Acceptance Criteria

### Visual Contrast

| Element | Minimum Contrast Ratio | Standard |
|---------|----------------------|----------|
| Critical UI Text (HP, Lives) | 7:1 | WCAG AAA |
| Standard UI Text (Score, Labels) | 4.5:1 | WCAG AA |
| Large Text (Titles >18px) | 3:1 | WCAG AA |
| Interactive Elements (Buttons) | 3:1 | WCAG AA |
| Player vs. Background | 5:1 | Custom |
| Enemy Projectiles vs. Background | 4:1 | Custom |

**Testing Tools:**
- WebAIM Contrast Checker
- Colour Contrast Analyser (CCA)

### UI Size & Scaling

| Metric | Default | Minimum (50% scale) | Maximum (200% scale) |
|--------|---------|---------------------|----------------------|
| Critical Text | 16px | 8px | 32px |
| Standard Text | 14px | 7px | 28px |
| Button Height | 40px | 20px | 80px |
| Icon Size | 32x32px | 16x16px | 64x64px |

**Validation:**
- Test at all scale increments (50%, 75%, 100%, 125%, 150%, 175%, 200%)
- No overlapping elements at any scale
- Text remains legible at minimum scale (user testing)

<div style="page-break-before: always;"></div>

### Performance Targets

| Metric | Target | Hardware Reference |
|--------|--------|-------------------|
| Frame Rate | 60 FPS stable | Mid-range PC (GTX 1060 / RX 580) |
| Loading Time | < 3s for level transitions | SSD storage |
| Network Latency Tolerance | Playable up to 150ms RTT | Multiplayer mode |

**Testing Methodology:**
- Use high-precision timer for latency measurements
- Test on minimum spec hardware
- 30-minute stress test with no frame drops

---

### Accessibility Feature Coverage

#### Pass/Fail Criteria

| Feature | Pass Criteria | Test Method |
|---------|---------------|-------------|
| **Full Remapping** | 100% of game actions remappable | Manual testing + automated check |
| **HUD Scaling** | All scales (50%-200%) functional without overlap | Visual inspection at all scales |
| **Colorblind Modes** | All 3 modes implemented + validation with simulator | Coblis/Color Oracle screenshots |
| **Subtitles** | 100% of audio cues have text equivalent | Audio track audit |
| **Keyboard Navigation** | All menus navigable without mouse | Unplug mouse test |
| **Epilepsy Warning** | Displayed on first launch | QA checklist |
| **Contrast Ratios** | All UI elements meet minimum contrast | Automated tool + manual spot checks |

#### Coverage Requirements

- **Remapping Coverage:** 100% of actions
- **Subtitle Coverage:** 100% of critical sounds, 90%+ of all sounds
- **Colorblind Validation:** All 3 presets must pass simulation test
- **Keyboard Navigation:** 100% of UI accessible without mouse

### Latency & Responsiveness

| Input Type | Action | Max Acceptable Latency |
|------------|--------|------------------------|
| Keyboard | Movement | 16ms (1 frame @ 60fps) |
| Keyboard | Shoot | 33ms (2 frames @ 60fps) |
| Menu Navigation | Button press → action | 100ms |

**Measurement:**
- Use high-speed camera to measure screen response
- Test on minimum and recommended hardware
- Average over 100 samples

---

## Developer Checklist

### Quick Reference for Implementation

Use this checklist to ensure compliance with all guidelines during development and code review.

#### Player Experience

- [ ] All weapons have satisfying visual feedback (muzzle flash, impact effects)
- [ ] All weapons have distinct audio feedback
- [ ] Boss enemy attacks are telegraphed with visual cues (flashing, animation)
- [ ] Boss enemy attacks are telegraphed with audio cues
- [ ] Telegraph duration meets minimum requirements (1000ms major)
- [ ] Player death sequence is dramatic and clear (1-2s)
- [ ] Difficulty progression follows documented pacing structure
- [ ] Power-ups appear after difficult sections

#### UI/HUD Readability

- [ ] Critical info (HP, lives) is in fixed top/bottom bars
- [ ] HUD does not overlap main play area (15% margins)
- [ ] Text meets minimum size requirements (16px critical, 14px standard)
- [ ] Text contrast meets WCAG AA (4.5:1) or AAA (7:1) where possible
- [ ] Screen effects respect duration limits (≤200ms shake, ≤150ms flash)
- [ ] UI animations are smooth and brief (≤300ms)

<div style="page-break-before: always;"></div>

#### Accessibility - Input

- [ ] All game actions are remappable (keyboard)
- [ ] Rebinding system detects and warns of conflicts
- [ ] Default key bindings are documented
- [ ] Toggle autofire option is implemented
- [ ] Input buffering is implemented (50ms window)

#### Accessibility - Visual

- [ ] HUD scaling is implemented (50%-200% in 25% increments)
- [ ] Scaling does not break layout at any increment
- [ ] Colorblind mode: Deuteranopia implemented
- [ ] Colorblind mode: Protanopia implemented
- [ ] Colorblind mode: Tritanopia implemented
- [ ] Colorblind modes validated with simulation tool
- [ ] High contrast mode implemented
- [ ] Epilepsy warning displayed on first launch

#### Accessibility - Audio

- [ ] Subtitles for all critical sound cues
- [ ] Subtitle text is scalable
- [ ] Subtitle background is semi-transparent
- [ ] Independent volume controls (master, music, SFX)
- [ ] Volume range is 0-100% with mute option
- [ ] Visual sound indicators are optional and implemented

#### Accessibility - Other

- [ ] Multiple difficulty levels implemented (Easy, Normal, Hard, Expert)
- [ ] Pause functionality works in single-player
- [ ] Tutorial introduces mechanics progressively

#### Acceptance Criteria

- [ ] All critical UI text meets 7:1 contrast ratio
- [ ] All standard UI text meets 4.5:1 contrast ratio
- [ ] Player vs. background contrast is ≥5:1
- [ ] Enemy projectiles vs. background contrast is ≥4:1
- [ ] Game maintains 60 FPS on reference hardware
- [ ] Input latency is < 50ms (measured)
- [ ] All accessibility features pass/fail tests are met
- [ ] Remapping coverage is 100%
- [ ] Subtitle coverage is ≥90%

---

## References

### Standards & Guidelines

- [WCAG 2.1 Guidelines](https://www.w3.org/WAI/WCAG21/quickref/) - Web Content Accessibility Guidelines
- [Game Accessibility Guidelines](http://gameaccessibilityguidelines.com/) - Industry best practices
- [AbleGamers Resources](https://ablegamers.org/) - Resources for accessible game design

### Testing Tools

- **Contrast Checkers:**
  - [WebAIM Contrast Checker](https://webaim.org/resources/contrastchecker/)
  - [Colour Contrast Analyser (CCA)](https://www.tpgi.com/color-contrast-checker/)

- **Colorblind Simulators:**
  - [Coblis](https://www.color-blindness.com/coblis-color-blindness-simulator/)
  - [Color Oracle](https://colororacle.org/)

- **Performance Profiling:**
  - Built-in engine profiler
  - Frame timing tools (see technical documentation)

### Internal Documentation

- [Contributing Guidelines](CONTRIBUTING.md)
- [Directory Structure](DIRECTORY_STRUCTURE.md)

<div style="page-break-before: always;"></div>

## Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-12-01 | Initial Draft | Complete guideline creation |

---

## Feedback & Contributions

This is a living document. If you have suggestions, concerns, or questions about these guidelines:

1. Open an issue on the project repository
2. Tag with `documentation` and `accessibility` labels
3. Reference the specific section and guideline

All team members are encouraged to contribute to improving these standards.

---

**Document Maintained By:** Design & UX Team  
**Review Cycle:** Quarterly or as needed for major updates  
**Next Review Date:** March 1, 2026
