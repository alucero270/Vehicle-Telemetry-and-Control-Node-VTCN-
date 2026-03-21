# ADR-002: Telemetry Framing

## Status

Accepted

## Context

VTCN must export telemetry to downstream consumers, including the Edge AI Diagnostics Platform, through a contract that can evolve without depending on internal C++ layouts.

## Decision

Telemetry uses a versioned frame envelope with explicit:

- frame boundary marker
- protocol version
- monotonic timestamp
- payload length
- integrity protection such as CRC32

The wire contract is documented separately from internal daemon types.

## Consequences

- downstream consumers can validate compatibility explicitly
- parser and round-trip tests can be written without target hardware
- breaking changes require deliberate versioning rather than silent drift

## Validation Boundary

This ADR establishes framing direction only. It does not claim that framing, transport, or interoperability has been validated on target hardware.
