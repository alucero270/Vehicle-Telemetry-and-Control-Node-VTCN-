# Failure Modes

## Purpose

This document captures the first-pass failure mode view for VTCN. It is intended to guide architecture decisions, future validation work, and observability design. It does not claim that any hardware-facing behavior has been validated in Phase 0.

## Failure Mode Table

| Failure mode | Detection signal | Likely impact | Intended handling direction |
| ------------ | ---------------- | ------------- | --------------------------- |
| Bad config | startup parse error, missing required key, invalid value range, unsupported module selection | daemon cannot start correctly or starts with incorrect wiring | fail fast during startup, emit clear error logs, reject invalid configuration before acquisition begins |
| CRC mismatch on inbound or outbound telemetry frame | protocol validation failure, CRC counter increment, explicit frame reject event | corrupted sample or message cannot be trusted; downstream gaps or invalid data markers may appear | drop or mark invalid frame, record integrity event, keep protocol compatibility rules explicit |
| Sensor disconnect | repeated read error, timeout, implausible flatline, device status change | missing or stale telemetry from affected signal path | surface degraded signal state upward, preserve distinction between missing and valid data, avoid silent substitution |
| Missing TCP peer or disconnected client | socket accept failure, send failure, connection reset, no active consumer | network telemetry stream unavailable while local processing may continue | treat network path as degraded but bounded, continue local logging where possible, expose degraded service state |
| MCU timeout or lost heartbeat | expected frame gap exceeded, heartbeat timeout, sequence stall | deterministic acquisition path becomes unavailable or stale | mark MCU-backed signals degraded, raise transport fault, keep Linux-side orchestration alive if safe |
| Invalid ADC input or out-of-range reading | out-of-range raw sample, conversion error, device read error, implausible bounds check | incorrect physical interpretation if consumed blindly | preserve invalid status, avoid burying policy inside HAL, let daemon decide downstream handling and logging |
| BBB reboot or power interruption | service restart, boot log discontinuity, monotonic clock reset, missing runtime continuity | temporary telemetry loss, broken in-memory state, interrupted network sessions | rely on restart-safe service design, log startup context, restore operation through normal bring-up path |
| Local storage unavailable or write failure | SQLite open failure, write error, latency spike, disk-full condition | evidence retention degraded; troubleshooting value drops even if live streaming continues | surface storage health separately, protect main loop from blocking indefinitely, continue bounded operation where possible |
| Protocol version mismatch between producer and consumer | version check failure, rejected handshake, incompatible frame parse | interoperability break between daemon and downstream client or diagnostics platform | fail explicitly, expose negotiated or expected version, require controlled versioned change rather than silent coercion |
| HAL device path missing or permission error | missing `/dev` or sysfs path, open failure, permission denied | affected interface cannot acquire data | fail interface initialization clearly, isolate the fault to the relevant module, avoid pretending the signal is healthy |

## Handling Principles

The intended architecture-level response to failures is:

- fail fast on configuration and compatibility errors
- degrade explicitly on partial runtime failures
- preserve evidence through logs, counters, and stored records where possible
- avoid silent data fabrication
- keep hardware I/O faults separated from daemon policy and protocol semantics

## Observability Expectations

These failure modes imply a later need for:

- startup diagnostics that identify configuration and dependency problems
- protocol integrity counters for CRC, parse, and version failures
- per-interface health reporting for HAL and MCU paths
- bounded service-health reporting for storage and network outputs
- retained evidence under `docs/test-results/` once hardware validation begins

## Host-Only Development Limitation

During Phase 0, this table is a planning artifact only. Host-side work can validate that software reacts coherently to synthetic config errors or pure logic failures, but it cannot validate real GPIO interrupts, ADC quality, UART timing, MCU heartbeat stability, or BeagleBone reboot behavior on target hardware. Those require later procedures, validation tools, and captured evidence.
