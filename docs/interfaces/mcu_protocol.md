# MCU Protocol

## Purpose

This document defines the contract boundary between the optional MCU coprocessor and the BeagleBone Black-hosted VTCN daemon. It covers transport intent, message direction, heartbeat expectations, and ownership boundaries at a high level.

The MCU protocol is separate from the external telemetry protocol. It exists to move deterministic acquisition results into the Linux node, not to expose a general-purpose application API.

## Responsibilities

The MCU-side transport contract is responsible for:

- carrying deterministic acquisition results from the MCU to the VTCN daemon
- carrying bounded status and fault signals from the MCU to the VTCN daemon
- providing a heartbeat so the daemon can detect transport loss or stalled acquisition
- preserving message boundaries, version identity, and integrity information
- keeping the coprocessor interface isolated from external ground or Edge AI protocols

## non-goals

The MCU protocol does not exist to:

- replace daemon-owned filtering, calibration, framing, storage, or networking
- turn the MCU into the owner of system orchestration
- expose private daemon internals to firmware
- promise real-time performance numbers before target validation
- become the same contract as the external telemetry stream

## Direction of Flow

The primary direction is MCU to BBB.

Expected MCU to BBB message families:

- acquisition data
- heartbeat or liveness status
- fault or status notification

Secondary BBB to MCU messages may exist, but they must remain bounded and minimal. Examples include:

- session start or stop
- configuration selection that affects acquisition mode
- explicit reset or resynchronization request

The Linux daemon remains the owner of policy. The MCU remains the owner of deterministic acquisition only.

## Framing Expectations

At a high level, MCU transport messages should carry:

- protocol version
- message type
- sequence or ordering signal
- payload length
- integrity check such as CRC

This keeps the daemon able to:

- detect malformed or truncated messages
- detect message loss or stalled transport
- reject incompatible protocol versions explicitly

Exact byte layout is deferred until implementation.

## Heartbeat Expectations

Heartbeat support is required from the first usable MCU integration.

Heartbeat expectations:

- the MCU emits a periodic liveness indication while the transport is healthy
- heartbeat loss must be distinguishable from normal idle data conditions
- the daemon treats missed heartbeats as a transport-health problem, not as valid quiet sensor behavior
- heartbeat timing thresholds are implementation and hardware dependent and must be validated on target before being treated as evidence

## Expected Inputs and Outputs

Inputs to the MCU protocol boundary:

- sampled or counted data produced by MCU acquisition logic
- MCU-generated health or fault status
- bounded daemon-issued control requests when explicitly supported

Outputs from the MCU protocol boundary:

- transport frames that the daemon can validate and ingest
- heartbeat state visible to daemon health monitoring
- explicit fault indicators for stale or degraded MCU-side acquisition

## Error-Handling Direction

Error handling should follow these rules:

- malformed frames are rejected explicitly
- CRC or integrity failures are surfaced upward and not silently ignored
- version mismatches fail clearly rather than being coerced
- heartbeat loss degrades MCU-backed signal trust rather than fabricating healthy data
- transport faults do not move filtering or calibration responsibility into the MCU layer

## Phase 0 Limitation

This document defines contract intent only. It does not claim validated serial timing, framing stability, or heartbeat behavior on real hardware.
