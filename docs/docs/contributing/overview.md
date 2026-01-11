---
title: Overview
---

# Contributing to R-Type

This section explains how to **contribute to the R-Type project**. It defines the
expected workflow, coding standards, and review process for anyone submitting
changes.

It is intended for:
- students working on the project
- contributors adding features or fixes
- reviewers validating code and documentation

The goal is to keep the codebase **consistent, readable, and maintainable**.

---

## What You Can Contribute

Contributions may include:
- bug fixes
- gameplay features
- networking or performance improvements
- documentation updates
- tooling or build improvements
- assets (sprites, sounds, data files)

All contributions are expected to follow the project’s structure and design
principles.

---

## Contribution Principles

- Respect the existing architecture and abstractions
- Keep changes focused and scoped
- Avoid mixing unrelated refactors with functional changes
- Prefer clarity and correctness over cleverness
- Maintain determinism and authority rules (server vs client)

If a change alters behavior, it must be documented.

---

## Documentation

Documentation is part of the codebase:
- technical docs explain *how the system works*
- user guide explains *how to use the system*
- contributing docs explain *how to change the system*

If you change behavior, update the relevant documentation in the same PR.

---

## Workflow Overview

1. Create a feature or fix branch from the main branch
2. Make incremental, well-scoped commits
3. Ensure the project builds and runs locally
4. Update documentation if required
5. Open a pull request for review

Do not commit directly to the main branch.

---

## Review Expectations

Pull requests are reviewed for:
- correctness and stability
- architectural consistency
- clarity and maintainability
- documentation accuracy

Reviews may request:
- code changes
- refactors
- additional documentation
- clarification comments

---

## Where to Go Next

- **Guidelines**: detailed coding and style rules
- **Pull Requests**: how to structure and submit a PR
- **Add Missile Sprite**: example contribution walkthrough
