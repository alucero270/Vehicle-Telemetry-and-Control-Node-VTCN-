# Telemetry Protocol

## Purpose

This document defines the planning baseline for the VTCN telemetry wire contract. It describes the versioned frame envelope that downstream consumers such as ground tooling and the Edge AI Diagnostics Platform are expected to parse.

This document is authoritative for external framing intent. It is not a definition of internal C++ types or in-memory object layout.

## Scope

In scope:

- frame structure overview
- versioning rules
- monotonic timestamp expectations
- payload length rules
- CRC and integrity behavior

Out of scope:

- full payload schema catalog
- daemon implementation details
- HAL behavior
- transport-specific service APIs

## Wire Contract vs Internal Types

The protocol contract is defined by on-wire fields and compatibility rules. Internal daemon structures, classes, or serialization helpers may change as long as they continue to emit frames that satisfy this document.

Consumers must not assume:

- a C++ struct layout
- host endianness outside documented encoding rules
- private daemon module names

## v1 Frame Envelope Overview

VTCN v1 frames are logically structured as:

```text
[magic][protocol_version][frame_type][sequence][monotonic_timestamp][payload_length][payload][crc32]
```

The exact byte encoding will be fixed during implementation, but the field order and purpose are part of the contract from day one.

| Field | Purpose | Contract rule |
| ----- | ------- | ------------- |
| `magic` | frame boundary and stream resynchronization | fixed constant at the start of each frame |
| `protocol_version` | compatibility boundary | required in every frame; breaking changes increment the version |
| `frame_type` | payload family identifier | lets a consumer interpret the payload without inspecting daemon internals |
| `sequence` | gap detection and ordering support | increases monotonically within a stream until wrap or stream restart |
| `monotonic_timestamp` | event ordering and age calculation | derived from a monotonic clock source, not wall-clock time |
| `payload_length` | payload boundary | declares the exact number of payload bytes that follow |
| `payload` | frame-specific content | interpreted according to `frame_type` and `protocol_version` |
| `crc32` | integrity check | used to detect corrupted frames before consumption |

## Versioning Strategy

The protocol is versioned from its first supported revision.

Rules:

- every frame carries a protocol version
- a breaking change requires a new version value
- a consumer that does not support the advertised version must fail explicitly rather than guess
- additive changes within a supported version should preserve parser safety through explicit lengths and frame typing

Versioning belongs to the protocol contract, not to internal daemon release numbers.

## Monotonic Timestamp Expectations

The timestamp field represents monotonic time suitable for ordering and latency reasoning.

Rules:

- ordering is based on a monotonic source rather than wall-clock time
- timestamps are expected to be non-decreasing within a healthy stream
- timestamp resets may occur after process restart, reboot, or new stream session
- consumers must not reinterpret monotonic timestamps as globally synchronized real time without a separate mapping

Wall-clock correlation may exist elsewhere in the system, but it is not the primary ordering contract for telemetry frames.

## Payload Length Rules

The payload length field exists so a receiver can determine frame boundaries without inferring size from payload contents.

Rules:

- `payload_length` declares the exact payload byte count
- receivers must reject frames where the available bytes do not match the declared length
- oversized frames must be rejected according to implementation limits rather than partially consumed
- zero-length payloads are only valid for frame types that explicitly allow them, such as future heartbeat-style frames

Length validation is mandatory even when the underlying transport is reliable.

## CRC and Integrity Expectations

CRC32 is the baseline integrity mechanism for v1 telemetry framing.

Rules:

- every frame carries a CRC32 value
- the CRC is computed over the framed contents defined by the protocol encoding
- receivers must validate CRC before treating a frame as good data
- frames with failed CRC must be rejected or marked invalid; they must not be silently accepted
- transport-level checksums are not a substitute for protocol-level integrity validation

Integrity failures should be observable through counters, logs, or equivalent diagnostics once implementation begins.

## Payload Families

The frame envelope is intended to support multiple payload families while keeping the transport contract stable.

Expected early families include:

- telemetry sample data
- health or status events
- heartbeat or keepalive signaling

This document does not freeze all future payload schemas. Payload definitions should be documented separately without breaking the frame envelope contract.

## Compatibility Notes

For Phase 0 planning, v1 should be treated as the first intended interoperable format, not as a promise that every future field decision is final. What is fixed now is the requirement for:

- explicit versioning
- explicit payload length
- monotonic timestamping for ordering
- protocol-level integrity checks
- separation between wire contract and internal implementation

## Phase 0 Limitation

This document defines contract intent only. Host-side work can validate parsers and framing logic later, but it does not validate real BeagleBone Black device timing, transport behavior, or hardware acquisition fidelity.
