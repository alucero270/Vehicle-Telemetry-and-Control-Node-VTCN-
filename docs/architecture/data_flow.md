# Data Flow

## Purpose

This document describes the intended high-level movement of data through VTCN. It defines ownership transitions between acquisition, daemon processing, storage, and downstream consumers without specifying low-level wire bytes.

## Primary Flow

The normal data path is:

```text
Physical sensor
    -> HAL or MCU acquisition
    -> VTCN telemetry daemon
    -> local logging / storage
    -> network telemetry export
    -> ground client and Edge AI Diagnostics Platform
```

The flow is intentionally directional. Hardware-facing modules push device state upward. Downstream systems consume versioned outputs and do not reach back into private implementation details.

## Flow Stages

| Stage | Owner | Input | Output | Notes |
| ----- | ----- | ----- | ------ | ----- |
| Sensor source | external hardware | physical state such as voltage, pulse train, serial data | raw electrical or device data | outside software ownership |
| Acquisition | HAL on BBB or optional MCU | raw device-facing data | raw samples, device events, transport frames, status | HAL = hardware I/O only; MCU = deterministic acquisition only |
| Daemon ingest | VTCN daemon | HAL readings or MCU-delivered frames | internal normalized signal records with timestamps and status | daemon owns filtering, calibration, framing, logging, and routing |
| Framing | protocol layer used by daemon | normalized signal records | versioned telemetry frames | protocol remains isolated from hardware modules |
| Local persistence | daemon storage path | normalized or framed telemetry plus runtime events | durable records, logs, later evidence | intended for debugging, analysis support, and audit trail |
| Network export | daemon network path | versioned telemetry frames and service health | bounded downstream stream or API response | supports ground tooling and Edge AI integration |
| Downstream consumption | ground client or Edge AI Diagnostics Platform | versioned telemetry and evidence | visualizations, diagnostics, workflow actions, incident evidence | outside VTCN implementation boundary |

## Acquisition Variants

### Direct BBB Path

Use this path when the BeagleBone Black can acquire the signal directly through Linux-managed interfaces such as ADC, GPIO, I2C, SPI, or UART.

```text
Sensor -> BBB HAL -> daemon -> SQLite / TCP -> downstream consumers
```

### MCU-Assisted Path

Use this path when timing determinism or capture rate is better handled by a coprocessor.

```text
Sensor -> MCU -> daemon transport input -> daemon -> SQLite / TCP -> downstream consumers
```

In this variant:

- the MCU owns deterministic acquisition only
- the daemon remains the owner of filtering, calibration, framing, storage, and networking
- the MCU does not replace the Linux application layer

## Data Ownership Transitions

Ownership changes at specific points in the system:

| Transition | Ownership change | Reason |
| ---------- | ---------------- | ------ |
| sensor to HAL or MCU | physical world to acquisition interface | convert external state into software-visible data |
| HAL or MCU to daemon | hardware-facing module to orchestration layer | centralize policy, calibration, framing, and logging |
| daemon to storage | volatile process state to persisted evidence | support replay, debugging, and post-run analysis |
| daemon to network output | internal processing to external contract | isolate internal implementation from downstream consumers |
| VTCN to Edge AI Diagnostics Platform | telemetry node to analysis platform | keep acquisition and diagnostics responsibilities separated |

## Integrity and Timing Expectations

The following expectations shape later interface work:

- timestamps should be monotonic-aware where ordering matters
- telemetry outputs should be versioned from the first supported frame
- integrity checks such as CRC belong to protocol validation paths, not to ad hoc downstream assumptions
- invalid, stale, or missing data must remain distinguishable from good data

Detailed field definitions belong in interface documents under `docs/interfaces/`.

## Storage and Network Branches

After daemon ingest, the system intentionally branches into two output paths:

- local storage for evidence capture, debugging, and replay-friendly records
- network telemetry export for ground tools and the Edge AI Diagnostics Platform

These branches serve different purposes. Storage is optimized for retained evidence and traceability. Network output is optimized for bounded interoperability and downstream consumption.

## Host-Only Development Limitation

Phase 0 work can document and scaffold this flow, and host-side tests can later validate pure logic such as frame handling. Phase 0 cannot prove that real BBB peripherals, MCU timing, ADC behavior, serial transport stability, or network behavior on target match this intended flow. Those claims require later hardware validation with documented procedures and captured evidence.
