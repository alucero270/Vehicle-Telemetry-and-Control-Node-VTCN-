# VTCN – Vehicle Telemetry & Control Node

Embedded Linux telemetry platform built on the **BeagleBone Black (ARM Cortex‑A8)**.

This project is intentionally structured as a **production‑style embedded systems engineering exercise**. All features must be implemented and validated on real hardware. No simulated claims.

VTCN is also designed to integrate with the **Edge AI Diagnostics Platform**, which consumes telemetry streams for analysis, diagnostics, and automation workflows.

---

# Project Purpose

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

The platform is also intended to support **real vehicle telemetry work**, beginning with the OM606 engine swap project (TPS and coolant telemetry).

---

# Relationship to the Edge AI Diagnostics Platform

VTCN acts as a **telemetry acquisition and transport node** for the Edge AI Diagnostics Platform.

Architecture relationship:

```
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
```

The **Edge AI Diagnostics Platform** is responsible for:

- signal analysis
- anomaly detection
- system diagnostics
- telemetry visualization

The **contract between VTCN and the Edge AI Diagnostics Platform** is defined in:

```
docs/interfaces/edge_ai_diagnostics_contract.md
```

This contract defines:

- telemetry frame format
- message versioning
- CRC integrity checks
- transport expectations
- compatibility guarantees

VTCN must follow this contract strictly to ensure interoperability.

---

# Development Principles

## Hardware‑First Validation

Every subsystem must be tested and documented on the **BeagleBone Black hardware**.

## Reproducibility

All builds, deployments, and validation procedures must be reproducible from a clean environment.

## Modular Architecture

System responsibilities are intentionally separated:

| Component | Responsibility |
|--------|--------|
| HAL | hardware I/O only |
| Telemetry Daemon | filtering, calibration, framing, networking |
| Protocol | versioned message format |
| MCU Coprocessor | deterministic signal capture |

## Evidence‑Driven Engineering

Each phase requires:

- logs
- validation output
- documented procedures

Evidence is stored under:

```
docs/test-results/
```

---

# Repository Structure

```
vtcn/

  README.md
  CMakeLists.txt

  docs/
    architecture/
      system_overview.md
      component_boundaries.md
      data_flow.md
      failure_modes.md

    interfaces/
      hal_adc.md
      hal_gpio.md
      hal_i2c.md
      hal_spi.md
      hal_uart.md
      mcu_protocol.md
      telemetry_protocol.md
      edge_ai_diagnostics_contract.md

    procedures/
      beaglebone_bringup.md
      cross_compile_setup.md
      adc_validation.md
      debugging_workflow.md

    adr/
      ADR-001-hal-separation.md
      ADR-002-telemetry-framing.md
      ADR-003-mcu-coprocessor.md

    ai/
      ai_context.md
      ai_rules.md
      project_summary.md

    test-results/

  toolchain/
    beaglebone-gcc.cmake
    build.sh
    deploy.sh

  vtcn-daemon/
    CMakeLists.txt

    include/
      vtcn/
        hal/
        telemetry/
        logging/
        storage/
        net/
        config/

    src/

    tests/
      unit/
      integration/
      fixtures/

  mcu-node/
    firmware/
    protocol/
    test-tools/

  proto/

  schema/

  systemd/

  lkm/
```

---

# Testing Strategy

Testing in VTCN is divided into three categories.

## Unit Tests

Framework:

```
GoogleTest
```

Purpose:

- validate pure logic
- config parsing
- frame encoding/decoding
- CRC calculation

Location:

```
vtcn-daemon/tests/unit/
```

## Integration Tests

Purpose:

- verify module interaction
- daemon startup validation
- protocol round-trip checks

Location:

```
vtcn-daemon/tests/integration/
```

## Hardware Validation

Hardware interfaces cannot be fully tested on host systems.

Instead:

- validation tools are created
- procedures are documented
- evidence is recorded

Documentation:

```
docs/procedures/
```

Evidence:

```
docs/test-results/
```

## Host-Side Baseline

During Phase 0 Virtual Development Mode, the host-only scaffold can be checked with:

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

These commands validate build wiring and host-side test scaffolding only. They do not constitute hardware validation on the BeagleBone Black.

---

# MCU Coprocessor (Arduino / Future STM32)

The BeagleBone Black remains the **primary embedded Linux platform**.

The MCU node acts as a **real-time coprocessor**.

### Responsibilities

- capture high-frequency pulse inputs
- deterministic timing via hardware timers
- analog signal conditioning
- watchdog functionality

### Architectural Rule

```
MCU = real-time acquisition
BBB = system orchestration
```

The MCU must never replace Linux application logic.

---

# Phase Roadmap

## Phase 1 – Platform Foundation

Objective:

Establish a stable ARM Linux development target.

Deliverables:

- Debian image flashed and boot validated
- SSH secured (key-only authentication)
- serial console operational
- documentation of boot process and exposed interfaces

Evidence Required:

```
uname -a

dmesg

/sys and /dev listings
```

---

## Phase 2 – Toolchain & Cross‑Compilation

Objective:

Build reproducible host → ARM workflow.

Deliverables:

- ARM cross compiler installed
- CMake toolchain configuration
- deploy scripts
- cross‑compiled binary running on BBB

---

## Phase 3 – Hardware Interface Layer (HAL)

Interfaces:

- ADC (IIO)
- GPIO
- I2C
- SPI
- UART

Deliverables:

- HAL modules
- validation tools
- calibration documentation

---

## Phase 4 – Debugging & Observability

Tools:

- gdb
- strace
- perf
- dmesg

---

## Phase 5 – Kernel Module

Deliverables:

- loadable kernel module
- `/dev/vtcn` device
- read/write interface

---

## Phase 6 – Productionization

Deliverables:

- systemd service
- watchdog integration
- structured logging

---

## Phase 7 – Yocto Migration

Deliverables:

- Yocto image
- custom recipe for daemon
- reproducible build steps

---

# Telemetry Architecture

High-Level Flow

```
Sensors
   │
   ▼
HAL Layer
   │
   ▼
Telemetry Daemon
   │
 ┌─┴───────────┐
 ▼             ▼
SQLite        TCP Stream
                 │
                 ▼
       Edge AI Diagnostics Platform
```

Telemetry frames must include:

- magic number
- protocol version
- monotonic timestamp
- payload length
- CRC32

Protocol definitions are stored under:

```
docs/interfaces/
```

---

# Definition of Success

VTCN is considered successful when:

- it runs reliably on ARM hardware
- it acquires real sensor data
- hardware interfaces are validated
- telemetry streams reach the Edge AI Diagnostics Platform
- services run under systemd
- procedures allow reproducible deployment

---

# Current Status

Phase: **Platform Bring‑up**

Next Action:

1. Flash Debian image to BBB microSD
2. Boot and verify kernel and interface exposure
3. Begin documenting Phase 1 bring-up procedure

---

# Author Intent

This project is designed to serve as a **portfolio‑grade embedded systems platform** demonstrating:

- embedded Linux platform engineering
- hardware interface development
- telemetry system architecture
- debugging and validation workflows
- professional engineering documentation

The project intentionally mirrors practices used in aerospace, robotics, and advanced automotive embedded systems development.
