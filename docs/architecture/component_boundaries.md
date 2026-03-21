# Component Boundaries

## Boundary Rules

The repository is organized around strict ownership boundaries. These rules are binding for implementation work:

- HAL = hardware I/O only
- daemon = filtering, calibration, framing, logging, networking, and storage orchestration
- protocol = versioned and isolated
- MCU = deterministic acquisition only

If a proposed change violates one of these boundaries, the architecture should be clarified first rather than worked around in code.

## Component Ownership Matrix

| Component | Owns | Non-goals | Primary Interfaces |
| --------- | ---- | --------- | ------------------ |
| HAL | Linux device interaction, register or sysfs or character-device access, device status, transport errors | filtering, calibration policy, business logic, telemetry session management, storage | BBB-facing APIs documented under `docs/interfaces/hal_*.md` |
| Telemetry daemon | config-driven module wiring, filtering, calibration, framing, logging, persistence, network transport, process lifecycle | direct register ownership, undocumented wire formats, MCU firmware behavior | internal C++ modules plus versioned protocol definitions under `docs/interfaces/` |
| Protocol definitions | on-wire field expectations, versioning rules, integrity expectations, compatibility boundaries | direct hardware access, storage policy, daemon lifecycle policy | telemetry and external contract documents under `docs/interfaces/` |
| MCU coprocessor | deterministic sampling or pulse capture, simple heartbeat reporting, bounded upstream data delivery to BBB | SQLite, TCP services, Linux application orchestration, high-level analytics | transport contract defined in `docs/interfaces/mcu_protocol.md` |
| Ground client | operator-facing consumption of exported telemetry, debug or observability tooling, external control workflows when explicitly supported | private daemon internals, direct HAL ownership, assuming unstable internal types are public API | documented external telemetry contracts |
| Edge AI Diagnostics Platform | downstream analysis, anomaly detection, evidence correlation, diagnostics workflows | direct hardware ownership, replacing VTCN acquisition, depending on undocumented daemon internals | documented interoperability contract under `docs/interfaces/` |

## Interface Direction

The intended direction of dependency is:

```text
Sensors -> HAL -> daemon -> protocol-framed outputs -> downstream clients
Sensors -> MCU -> daemon -> protocol-framed outputs -> downstream clients
```

This direction matters:

- HAL reports device-facing state upward; it does not pull in daemon policy
- the daemon may consume HAL and MCU inputs, but it should not bury protocol definitions inside hardware modules
- downstream clients consume versioned contracts, not internal C++ object layouts

## Non-Goals by Boundary

### HAL Boundary

HAL modules must not absorb policy because that makes hardware behavior harder to test and harder to port.

HAL non-goals:

- filtering noisy signals
- applying calibration policy
- deciding network publication behavior
- owning retry strategy beyond local device interaction requirements

### Daemon Boundary

The daemon is the orchestration layer, not the place to hide undocumented hardware assumptions.

Daemon non-goals:

- direct manipulation of hardware through ad hoc shell calls outside HAL ownership
- redefining protocol compatibility per call site
- placing deterministic timing requirements onto Linux threads that belong on the MCU

### Protocol Boundary

Protocol definitions exist to stabilize interoperability.

Protocol non-goals:

- embedding hardware access rules
- depending on one internal class layout
- changing incompatibly without explicit versioning

### MCU Boundary

The MCU exists to support deterministic acquisition, not to become a second application platform.

MCU non-goals:

- storage and log management
- TCP service hosting
- analytics, calibration policy, or operator workflow logic

## Relationship to Edge AI Diagnostics

The Edge AI Diagnostics Platform is a downstream system, not a hidden submodule of VTCN. VTCN should expose bounded, documented interfaces for telemetry and evidence exchange. Analysis and diagnostics logic belong on the diagnostics side of the boundary unless an ADR explicitly assigns something else.

## Phase 0 Constraint

These boundaries apply during Virtual Development Mode as well as on target hardware. In Phase 0, host-side builds and documentation can validate structure and intent, but they do not validate real hardware behavior. Any claim about BBB peripherals, MCU timing, or transport stability requires later target-side evidence captured through documented procedures.
