VTCN – Vehicle Telemetry & Control Node

Embedded Linux telemetry platform built on the BeagleBone Black (ARM Cortex-A8).

This project is intentionally structured as a **production‑style embedded systems engineering exercise**. All features must be implemented and validated on real hardware. No simulated claims.

VTCN is also designed to integrate with the **Edge AI Diagnostics Platform**, which consumes telemetry streams for analysis, diagnostics, and automation workflows.
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
      virtual_development_mode.md
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
Hardware-First Validation

All production functionality must be validated on the BeagleBone Black or designated MCU hardware.

Reproducibility

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

The MCU must never replace Linux application logic.

---

Repository Structure

vtcn/

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
- performed on BeagleBone Black
- requires documented procedures and captured evidence

Host-Side Baseline

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

---

MCU Coprocessor

Tools:

- gdb
- strace
- perf
- dmesg

The MCU node provides deterministic real-time capabilities.

MCU = real-time acquisition
BBB = system orchestration

Responsibilities:

- high-frequency pulse capture
- hardware-timed measurements
- watchdog functionality

---

Phase Roadmap

Deliverables:

- loadable kernel module
- `/dev/vtcn` device
- read/write interface
Phase 1 – Platform Foundation

- BBB bring-up
- SSH + serial validation

Phase 2 – Toolchain

- cross-compilation
- deploy workflow

Phase 3 – HAL

Deliverables:

- systemd service
- watchdog integration
- structured logging
- ADC, GPIO, I2C, SPI, UART

Phase 4 – Debugging

- gdb, strace, perf

Phase 5 – Kernel Module

Deliverables:

- Yocto image
- custom recipe for daemon
- reproducible build steps
- "/dev/vtcn" interface

Phase 6 – Productionization

- systemd service
- logging

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

- it runs reliably on ARM hardware
- it acquires real sensor data
- hardware interfaces are validated
- telemetry streams reach the Edge AI Diagnostics Platform
- services run under systemd
- procedures allow reproducible deployment
- runs reliably on ARM hardware
- acquires real sensor data
- validates hardware interfaces
- streams telemetry successfully
- operates under systemd
- supports reproducible deployment

---

Current Status

Phase: Virtual Development Mode (Phase 0)

Phase: **Virtual Development Mode (Phase 0)**

Next Action:

1. Continue host-side scaffolding, interface work, and pure-logic tests
2. Prepare Phase 1 bring-up and validation procedures
3. Defer BeagleBone Black hardware claims until target evidence exists
Next Actions:

1. Complete host-side prototypes and validation
2. Finalize bring-up procedures
3. Begin BeagleBone Black integration

---

Author Intent

This project serves as a portfolio-grade embedded systems platform demonstrating:

This project is designed to serve as a **portfolio‑grade embedded systems platform** demonstrating:

- embedded Linux platform engineering
- hardware interface development
- telemetry system architecture
- debugging and validation workflows
- professional engineering documentation

The project intentionally mirrors practices used in aerospace, robotics, and advanced automotive embedded systems development.
- embedded Linux engineering
- hardware interface development
- telemetry architecture
- debugging and validation workflows
- professional documentation practices

The structure intentionally mirrors workflows used in aerospace, robotics, and advanced automotive systems.
