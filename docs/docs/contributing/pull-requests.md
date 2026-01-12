---
title: Pull Requests
---

# Pull Request Guidelines

This document defines the **rules and expectations for pull requests (PRs)**.
PRs are the only accepted way to merge changes into the main branch.

The goal is to keep reviews efficient, readable, and predictable.

---

## General Rules

- All changes **must** go through a pull request
- Direct commits to the main branch are forbidden
- Every PR must be reviewable in a reasonable amount of time
- A PR is not complete until it is **approved**

---

## Size and Scope

### Line limit
- A pull request must not exceed **1500 changed lines** (added + removed)
- If your change is larger:
  - split it into multiple PRs
  - or submit preparatory refactors separately

Large PRs are hard to review and are likely to be rejected.

---

### Scope discipline
A PR should:
- address **one topic**
- avoid mixing refactors, formatting, and features
- keep unrelated changes out

If two changes can be merged independently, they belong in different PRs.

---

## Commit Conventions

### Commit message format

Use a clear, conventional format:

```
<type>(scope): short description
```

Examples:
- `feat(network): add UDP bind validation`
- `fix(auth): handle expired session tokens`
- `docs(client): document state synchronization`
- `refactor(server): simplify room lifecycle`

Recommended types:
- `feat`
- `fix`
- `docs`
- `refactor`
- `perf`
- `test`
- `chore`

Rules:
- use present tense
- be concise
- no jokes in commit messages
- no “misc”, “stuff”, or “wip” commits

---

### Commit hygiene
- keep commits small and logical
- avoid squashing unrelated changes
- fixups should be squashed before review if requested

---

## Pull Request Description

Every PR **must** include a clear description.

### Required sections

- **What**
  - what the PR changes
  - what part of the system is affected

- **Why**
  - why the change is needed
  - what problem it solves

- **How**
  - high-level explanation of the approach
  - important design decisions

- **Impact**
  - runtime behavior changes
  - backward compatibility notes
  - risk areas (networking, auth, persistence, etc.)

If behavior changes, document it.

---

## Labels

Every PR must be labeled.

Required labels (at least one):
- `client`
- `server`
- `network`
- `auth`
- `docs`
- `ui`
- `engine`

Optional labels:
- `breaking-change`
- `refactor`
- `performance`
- `security`
- `cleanup`

Labels help reviewers route the PR correctly.

---

## Review and Approval

### Approval rules
- At least **one approval** is required
- The approval must include:
  - a short technical acknowledgment
  - **a funny meme** (mandatory!!)

Yes, this is intentional.  
If the PR has no meme, it is not approved.

### Example meme:
![img.png](im.png)
---

### Review expectations
Reviewers may ask for:
- code changes
- simplifications
- documentation updates
- test additions

Address review comments explicitly.
Do not ignore feedback.

---

## CI and Checks

Before approval:
- the project must build
- existing tests must pass (if applicable)
- no new warnings should be introduced

A PR that breaks the build will not be merged.

---

## Documentation Requirements

If the PR:
- changes architecture
- changes runtime behavior
- changes protocols or flows

Then:
- technical documentation **must** be updated
- user-facing changes **must** be reflected in the user guide

Docs and code must stay in sync.

---

## What Will Get a PR Rejected

- exceeds 1500 lines without justification
- unclear description
- missing labels
- missing approval (or missing meme)
- mixed unrelated changes
- undocumented behavior changes
- broken build or tests

---

## Final Notes

- A clean PR saves everyone time
- Small, focused changes are preferred
- If in doubt, ask before implementing a large change

A good PR is easy to review, easy to reason about, and easy to maintain !
