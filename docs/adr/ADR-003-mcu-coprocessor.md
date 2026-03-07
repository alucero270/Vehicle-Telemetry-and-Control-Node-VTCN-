# ADR-003: MCU Coprocessor

## Status

Accepted

## Context

Some acquisition tasks fit poorly on a Linux scheduler alone, especially when deterministic timing or pulse capture is required.

## Decision

An optional MCU coprocessor may be used for deterministic acquisition support.

The MCU is limited to:

- deterministic sampling or pulse capture
- bounded status reporting
- heartbeat or liveness signaling

The MCU does not replace:

- daemon-owned orchestration
- storage
- networking
- external telemetry protocol ownership

The primary data direction is MCU to BBB, with only bounded control messages in the reverse direction when needed.

## Consequences

- Linux remains the system orchestration layer
- deterministic acquisition can be isolated behind a documented transport boundary
- firmware scope stays controlled and reviewable

## Validation Boundary

This ADR records the intended architecture. It is not proof of MCU timing, heartbeat stability, or transport behavior on real hardware.
