# Edge AI Diagnostics Contract

## Purpose

This document defines the interoperability boundary between VTCN and the Edge AI Diagnostics Platform.

VTCN is the telemetry acquisition and transport node.
The Edge AI Diagnostics Platform is the downstream analysis and diagnostics consumer.

The contract exists so both sides can be implemented independently against a stable external interface rather than against private code structure.

## Relationship to the Telemetry Protocol

The telemetry frame envelope is defined in:

`docs/interfaces/telemetry_protocol.md`

This document builds on that framing contract and defines what the Edge AI Diagnostics Platform may assume about VTCN outputs at an integration level.

## VTCN Responsibilities

VTCN is responsible for:

- acquiring or receiving telemetry inputs from HAL modules and optional MCU transport
- normalizing data into daemon-owned records before export
- emitting versioned telemetry outputs
- preserving integrity information such as CRC validation outcome
- distinguishing valid, invalid, stale, and missing data states where applicable
- exposing bounded health and evidence signals for downstream analysis

VTCN is not responsible for downstream diagnostic conclusions.

## Edge AI Diagnostics Platform Responsibilities

The Edge AI Diagnostics Platform is responsible for:

- consuming documented VTCN outputs without relying on private daemon internals
- validating protocol version support before analysis
- respecting integrity and data-quality signals rather than assuming all received data is good
- correlating telemetry with higher-level diagnostics, operator workflows, or automation
- degrading explicitly when input data, versions, or evidence sources are unavailable

The Edge AI Diagnostics Platform is not the owner of direct hardware access or Linux-side acquisition policy.

## Compatibility Assumptions

The expected compatibility model is:

- VTCN exports a versioned telemetry contract from day one
- the Edge AI Diagnostics Platform consumes supported versions explicitly
- unknown or unsupported versions are rejected rather than guessed
- compatibility is defined at the contract level, not by shared source code or C++ types
- downstream consumers may skip unknown frame types only when the framing rules and payload length permit safe skipping

This allows VTCN and the diagnostics platform to evolve independently while still sharing a bounded integration point.

## Data and Timing Expectations

The Edge AI Diagnostics Platform may assume the following about telemetry exported by VTCN:

- each frame identifies its protocol version
- each frame carries a monotonic timestamp suitable for in-stream ordering
- each frame declares its payload length
- each frame carries protocol-level integrity protection
- ordering within a stream is reasoned about using monotonic timestamps and sequence progression rather than wall-clock assumptions alone

The diagnostics side must also assume:

- stream restarts can reset sequence and monotonic time origin
- missing data is different from invalid data
- lack of current telemetry is not proof that the monitored system is healthy

## Transport Expectations

This contract is intentionally transport-aware but not transport-bound.

Transport bindings may evolve over time, but any supported binding must preserve:

- frame byte fidelity
- frame ordering within a stream
- bounded error reporting
- explicit version visibility to the consumer

Examples of acceptable bindings may include a daemon-managed TCP stream or recorded export files. The interoperability contract is the frame content and compatibility behavior, not a specific internal daemon API.

## Integrity Expectations

Integrity handling is part of the interoperability boundary.

Rules:

- VTCN must not present corrupted frames as good data
- integrity failures must remain visible to downstream consumers through rejection, invalid markers, counters, or logs
- the Edge AI Diagnostics Platform must validate or trust only already-validated protocol data according to the defined transport path
- unsupported versions, bad CRC, or malformed lengths are explicit contract failures, not soft warnings

## non-goals

This contract does not define:

- direct hardware control from the Edge AI Diagnostics Platform
- private daemon RPCs or internal object layouts
- a complete historical query API surface
- every future payload schema
- any claim that host-only Phase 0 work validates target hardware behavior

## Phase 0 Limitation

In Phase 0, this contract is a design and implementation target only. Host-side document and parser work may validate compatibility logic later, but no current artifact should be treated as proof that BeagleBone Black hardware acquisition, MCU timing, or target-network behavior has been validated.
