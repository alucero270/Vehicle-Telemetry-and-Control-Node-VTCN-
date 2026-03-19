VTCN – Vehicle Telemetry & Control Node

Embedded Linux telemetry platform built on the BeagleBone Black (ARM Cortex-A8).

This project is intentionally structured as a production-style embedded systems engineering exercise.

All production features must be implemented and validated on real hardware.
Host-side simulations are used only for pre-integration validation and are never presented as completed hardware functionality.

VTCN is designed to integrate with the Edge AI Diagnostics Platform, which consumes telemetry streams for analysis, diagnostics, and automation workflows.

---

Project Purpose

VTCN exists to demonstrate real-world embedded Linux competencies while providing a telemetry node suitable for robotics, automotive, and aerospace-style systems.

Core competencies demonstrated:

- ARM-based Linux bring-up
- Cross-compilation workflows (x86 → ARM)
- Hardware interface validation (GPIO, I2C, SPI, UART, ADC)
- Telemetry protocol design
- Structured logging (SQLite)
- TCP telemetry streaming
- Low-level debugging (gdb, strace, perf, dmesg)
- Linux kernel module development
- systemd service hardening
- Reproducible builds and validation procedures

The platform is also intended to support real vehicle telemetry work, beginning with the OM606 engine swap project (TPS and coolant telemetry).

---

Relationship to the Edge AI Diagnostics Platform

VTCN acts as a telemetry acquisition and transport node.

Physical Sensors
      │
      ▼
HAL (BBB / MCU)
      │
      ▼
VTCN Telemetry Daemon
      │
      ├── SQLite Local Logging
      │
      └── TCP Telemetry Stream
              │
              ▼
Edge AI Diagnostics Platform
              │
              ▼
Diagnostics / Analysis / Automation

The Edge AI platform is responsible for:

- signal analysis
- anomaly detection
- system diagnostics
- telemetry visualization

The interface contract is defined in:

docs/interfaces/edge_ai_diagnostics_contract.md

---

Development Principles

Hardware-First Validation

All production functionality must be validated on the BeagleBone Black or designated MCU hardware.

Reproducibility

All builds, deployments, and validation procedures must be reproducible from a clean environment.

Modular Architecture

Component| Responsibility
HAL| hardware I/O only
Telemetry Daemon| filtering, calibration, framing, networking
Protocol| versioned message format
MCU Coprocessor| deterministic signal capture

Evidence-Driven Engineering

Each phase requires:

- logs
- validation output
- documented procedures

Evidence is stored under:

docs/test-results/

---

Phase 0 – Prototype Work (Virtual Development Mode)

To reduce integration risk before hardware bring-up, VTCN includes host-side prototypes for validating pure logic and timing-sensitive behavior.

Examples:

- signal emulation for pulse-based sensors
- RPM/frequency estimation
- telemetry frame encoding/decoding
- CRC validation
- fault injection and robustness testing

These prototypes:

- are deterministic and testable
- run on host systems
- do not represent completed hardware functionality

---

Active Prototype – Crank Signal Emulation & RPM Estimation

A Phase 0 prototype has been implemented to validate pulse-based signal processing prior to hardware integration.

Purpose

Simulate a crankshaft position sensor signal (36-1 tooth pattern) and validate RPM estimation logic under controlled conditions.

Capabilities

- software-generated pulse signal
- edge detection and interval measurement
- RPM estimation from pulse timing
- configurable noise and jitter injection
- expected vs measured error reporting

Example Output

Run Configuration
- Target RPM: 2200
- Duration: 5.0s
- Noise: 0.5% jitter

Results
- Measured RPM: 2189.4
- Average Error: 0.48%
- Signal Stability: PASS

Architectural Role

This prototype validates logic that will later operate on:

MCU → BBB HAL → Telemetry Daemon

It is strictly a host-side validation tool.

Next Steps

- integrate logic into HAL-backed signal acquisition
- validate against real sensor input on BeagleBone Black

---

Repository Structure

vtcn/

  README.md
  CMakeLists.txt

  docs/
    architecture/
    interfaces/
    procedures/
    adr/
    ai/
    test-results/

  toolchain/

  vtcn-daemon/
    include/vtcn/
    src/
    tests/

  mcu-node/
  proto/
  schema/
  systemd/
  lkm/

---

Testing Strategy

Unit Tests

- pure logic validation
- protocol encoding/decoding
- CRC checks

Integration Tests

- module interaction
- daemon startup
- pipeline validation

Hardware Validation

- performed on BeagleBone Black
- requires documented procedures and captured evidence

Host-Side Baseline

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

---

MCU Coprocessor

The MCU node provides deterministic real-time capabilities.

MCU = real-time acquisition
BBB = system orchestration

Responsibilities:

- high-frequency pulse capture
- hardware-timed measurements
- watchdog functionality

---

Phase Roadmap

Phase 1 – Platform Foundation

- BBB bring-up
- SSH + serial validation

Phase 2 – Toolchain

- cross-compilation
- deploy workflow

Phase 3 – HAL

- ADC, GPIO, I2C, SPI, UART

Phase 4 – Debugging

- gdb, strace, perf

Phase 5 – Kernel Module

- "/dev/vtcn" interface

Phase 6 – Productionization

- systemd service
- logging

Phase 7 – Yocto

- custom embedded image

---

Telemetry Architecture

Sensors → HAL → Telemetry Daemon → Storage / Network → Edge AI Platform

Telemetry frames include:

- magic number
- protocol version
- timestamp
- payload length
- CRC32

---

Definition of Success

VTCN is successful when:

- runs reliably on ARM hardware
- acquires real sensor data
- validates hardware interfaces
- streams telemetry successfully
- operates under systemd
- supports reproducible deployment

---

Current Status

Phase: Virtual Development Mode (Phase 0)

Next Actions:

1. Complete host-side prototypes and validation
2. Finalize bring-up procedures
3. Begin BeagleBone Black integration

---

Author Intent

This project serves as a portfolio-grade embedded systems platform demonstrating:

- embedded Linux engineering
- hardware interface development
- telemetry architecture
- debugging and validation workflows
- professional documentation practices

The structure intentionally mirrors workflows used in aerospace, robotics, and advanced automotive systems.