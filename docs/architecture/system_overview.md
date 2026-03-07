# System Overview

## Purpose

VTCN (Vehicle Telemetry & Control Node) is an embedded Linux telemetry platform centered on the BeagleBone Black (BBB). Its role is to acquire real-world signals, package them into a versioned telemetry contract, persist useful evidence locally, and stream telemetry to downstream consumers.

The project has two parallel goals:

- demonstrate production-style embedded Linux engineering on a constrained ARM target
- provide a reusable telemetry node for vehicle, robotics, and similar field systems

VTCN is part of a larger workflow that also includes the Edge AI Diagnostics Platform. VTCN is responsible for deterministic acquisition, framing, logging, and transport. The Edge AI Diagnostics Platform is responsible for downstream analysis, diagnostics, visualization, and automation workflows.

## System Context

At a high level, the system boundary is:

```text
Physical Sensors
    |
    v
HAL (BBB direct I/O and/or MCU-facing transport)
    |
    v
VTCN Telemetry Daemon
    |-- Local logging / storage
    |-- Network telemetry export
    |
    v
Ground Client and Edge AI Diagnostics Platform
```

VTCN does not exist to perform fleet-scale analytics or high-level diagnostic reasoning on the node itself. Its job is to move trustworthy telemetry and operational evidence from physical interfaces into stable software contracts.

## Major Components

### HAL

HAL = hardware I/O only.

The hardware abstraction layer owns direct interaction with BBB peripherals and Linux device interfaces such as ADC, GPIO, I2C, SPI, and UART. HAL modules expose device-facing reads, writes, status, and error reporting. They do not own filtering, calibration policy, or telemetry framing.

### Telemetry Daemon

The telemetry daemon is the Linux application layer that orchestrates the node. It owns configuration-driven wiring, signal handling policy, filtering, calibration, framing, logging, storage, and network publication.

### Protocol Layer

The protocol layer defines versioned external contracts. It isolates on-wire compatibility from internal C++ types and implementation changes so the daemon, ground tooling, and Edge AI consumers can evolve without undocumented breakage.

### MCU Coprocessor

The MCU coprocessor is optional and exists to handle deterministic acquisition tasks that are not a good fit for Linux scheduling alone. Typical examples include precise pulse timing, watchdog behavior, or simple time-critical capture. The MCU is not the owner of system orchestration, persistence, or network behavior.

### Ground Client

The ground client represents operator- or developer-facing software that consumes VTCN outputs for inspection, debugging, or control workflows defined elsewhere. It is downstream of VTCN and should rely on documented interfaces rather than private daemon internals.

### Edge AI Diagnostics Platform

The Edge AI Diagnostics Platform is a downstream consumer of telemetry and evidence produced by VTCN. It performs analysis, anomaly detection, diagnostics, and human-facing workflow support. It is outside the VTCN implementation boundary and must integrate through documented contracts.

## Relationship to the Edge AI Diagnostics Platform

VTCN supplies telemetry and operational evidence to the Edge AI Diagnostics Platform. That relationship is intentionally one of bounded interoperability:

- VTCN owns data acquisition, local evidence capture, and transport
- the Edge AI Diagnostics Platform owns diagnostics and higher-level interpretation
- compatibility is maintained through versioned interfaces defined under `docs/interfaces/`

This separation keeps the node implementation focused on deterministic acquisition and transport while allowing downstream tooling to evolve independently.

## Phase 0 Development Position

Phase 0 establishes the project truth layer before target hardware is available. During this phase, Virtual Development Mode is in effect:

- architecture and interface documents are authoritative
- host-side build scaffolding and unit tests are allowed
- protocol and daemon skeleton work are allowed
- hardware validation claims are not allowed

Real hardware validation is deferred until the BeagleBone Black and supporting interfaces are available. Host-only work in Phase 0 can validate documentation quality, build wiring, and pure logic, but it cannot be used as evidence that GPIO, I2C, SPI, UART, ADC, or MCU timing behavior works on target.

## System-Level Non-Goals

The VTCN repository is not intended to:

- collapse HAL, daemon, protocol, and MCU concerns into one module
- replace the Edge AI Diagnostics Platform with on-node diagnostic policy
- claim hardware support before target-side validation evidence exists
- treat host-only scaffolding as proof of BeagleBone Black behavior
