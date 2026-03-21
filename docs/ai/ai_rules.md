# AI Rules

## Source of Truth

The repository is the canonical memory layer for VTCN.

Authoritative files:

- `README.md`
- `docs/architecture/*`
- `docs/interfaces/*`
- `docs/procedures/*`
- `docs/adr/*`
- `docs/ai/*`

If chat context conflicts with repository documents, repository documents win.

## Operating Rules

- work in small, reviewable increments
- prefer documentation-first clarification when architecture is still forming
- do not invent structure that conflicts with documented boundaries
- update docs when an interface, procedure, or architecture decision changes
- keep host-only work clearly separated from real hardware validation

## Architecture Guardrails

- HAL = hardware I/O only
- daemon = filtering, calibration, framing, logging, networking, and storage orchestration
- protocol = versioned and isolated
- MCU = deterministic acquisition only

Do not move policy logic into HAL modules.

## Validation Rules

- do not claim hardware validation before target hardware exists and procedures are run
- host-side builds and tests validate scaffolding or pure logic only
- hardware-facing claims require documented procedure steps and evidence under `docs/test-results/`
- no simulated success claims

## Context Discipline

- shared chat memory cannot be assumed
- each session should re-read the relevant repository documents before making changes
- do not rely on undocumented prior intent
- call out ambiguity instead of filling gaps with speculative implementation
