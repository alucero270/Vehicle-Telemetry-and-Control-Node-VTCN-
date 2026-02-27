# VTCN – Vehicle Telemetry & Control Node

Embedded Linux telemetry platform built on the **BeagleBone Black (ARM Cortex‑A8)**.

This project is intentionally structured as a production-style embedded systems engineering exercise. All features must be implemented and validated on real hardware. No simulated claims.

---

# Project Purpose

VTCN exists to demonstrate real-world embedded Linux competencies:

* ARM-based Linux bring-up
* Cross-compilation workflows (x86 → ARM)
* Hardware interface validation (GPIO, I2C, SPI, UART, ADC)
* Telemetry framing and transport (TCP)
* Structured logging (SQLite)
* Low-level debugging (gdb, strace, perf, dmesg)
* Linux kernel module development
* systemd service hardening
* Reproducible build and deployment procedures

This platform also serves as a telemetry foundation for physical vehicle integration (OM606 engine swap), starting with TPS and coolant temperature acquisition.

---

# Development Principles

1. **Hardware-first validation**
   Every subsystem must be tested and documented on the BeagleBone Black.

2. **Reproducibility**
   All builds, deployments, and tests must be repeatable via documented procedures.

3. **Modular Architecture**

   * HAL = hardware I/O only
   * Daemon = policy, filtering, calibration, networking
   * Protocol = versioned and isolated

4. **Evidence-Driven Engineering**
   Each phase requires captured logs, outputs, and documented test results.

---

# Repository Structure

```
vtcn/
  README.md

  docs/
    architecture/
    interfaces/
    procedures/
    test-results/

  toolchain/
    beaglebone-gcc.cmake
    build.sh
    deploy.sh

  vtcn-daemon/
    src/
    include/
    tests/

  mcu-node/                # Arduino / MCU coprocessor firmware
    firmware/
    protocol/
    test-tools/

  ground-ui/

  proto/

  schema/

  systemd/

  lkm/
```

---

# MCU Coprocessor (Arduino / Future STM32)

The BeagleBone Black remains the primary embedded Linux platform.
The MCU node acts strictly as a **real-time coprocessor and signal conditioner**.

## Purpose

The MCU may be used to:

* Capture high-frequency pulse inputs (RPM, speed sensors)
* Perform deterministic timing using hardware timers
* Condition and filter noisy analog automotive signals
* Provide a watchdog/reset mechanism for the BBB
* Expose a clean UART/I2C/SPI protocol to the Linux daemon

## Architectural Rule

* MCU = real-time acquisition only
* BBB daemon = logging, calibration policy, networking, storage

The MCU must never replace Linux application logic.

## Deliverables (When Activated in Later Phases)

* Defined binary protocol between MCU ↔ BBB
* Documented timing characteristics
* Validation logs comparing raw vs conditioned signals
* Failure-mode behavior (BBB reset, heartbeat timeout)

---

```
vtcn/
README.md

docs/
architecture/
interfaces/
procedures/
test-results/

toolchain/
beaglebone-gcc.cmake
build.sh
deploy.sh

vtcn-daemon/
src/
include/
tests/

ground-ui/

proto/

schema/

systemd/

lkm/

```

---

# Phase Roadmap

## Phase 1 – Platform Foundation

**Objective:** Establish a stable ARM Linux development target.

Deliverables:
- Debian image flashed and boot validated
- SSH secured (key-only authentication)
- Serial console operational
- Documentation of boot process and exposed interfaces

Evidence Required:
- `uname -a`
- `dmesg` excerpts
- Interface listings (`/sys`, `/dev`)
- Bring-up procedure document

---

## Phase 2 – Toolchain & Cross-Compilation

**Objective:** Build reproducible host → ARM workflow.

Deliverables:
- ARM cross-compiler installed on host
- CMake toolchain file
- Cross-compiled C++ binary running on BBB
- Automated deploy script

Evidence Required:
- `file <binary>` showing ARM ELF
- Successful remote execution
- Documented toolchain configuration

---

## Phase 3 – Hardware Interface Layer (HAL)

**Objective:** Implement structured hardware abstraction modules.

Interfaces:
- ADC (IIO sysfs)
- GPIO (interrupt + polling comparison)
- I2C (real sensor)
- SPI (external ADC)
- UART (mock or loopback)

Deliverables:
- Clean C/C++ interface modules
- Standalone test utilities for each interface
- Calibration documentation
- Timestamped logging validation

---

## Phase 4 – Debugging & Observability

**Objective:** Demonstrate professional debugging capability.

Tools:
- gdb (remote)
- strace
- perf
- dmesg

Deliverables:
- Documented debugging workflow
- Performance profiling results
- Memory usage characterization
- Failure-mode test documentation

---

## Phase 5 – Kernel Module

**Objective:** Implement minimal Linux kernel integration.

Requirements:
- Loadable kernel module (LKM)
- `/dev/vtcn` device node
- Basic read/write interface
- dmesg logging validation

Deliverables:
- Kernel module source
- Build procedure
- Load/unload validation logs

---

## Phase 6 – Productionization

**Objective:** Harden system behavior.

Deliverables:
- systemd-managed service
- Watchdog mechanism
- Structured logging
- Controlled configuration file
- Graceful signal handling

---

## Phase 7 – Yocto Migration

**Objective:** Build custom embedded Linux image.

Deliverables:
- Yocto build documentation
- Custom layer + recipe
- Reproducible image build steps
- Image flashing procedure
- Verified boot + service startup

---

# Telemetry Architecture Overview

High-Level Flow:

```

Sensors → HAL → Telemetry Daemon →
→ SQLite Logging
→ TCP Stream → Ground Client
→ (Future) CAN / Replay

```

Telemetry frames must include:
- Magic number
- Version
- Timestamp (monotonic)
- Payload length
- CRC32

Protocol definitions must live in `/proto` and be versioned.

---

# Definition of Success

VTCN is considered successful when:

- It runs reliably on ARM hardware
- It acquires physical sensor data
- It exposes validated device interfaces
- It runs as a managed Linux service
- It includes documented bring-up and debugging workflows
- All procedures are reproducible from clean setup

---

# Current Status

> Phase: Not Started / Platform Bring-up

Next Action:
1. Flash Debian image to BBB microSD.
2. Boot and verify kernel and interface exposure.
3. Begin documenting Phase 1 bring-up procedure.

---

# Author Intent

This project is designed to serve as a credible embedded systems portfolio artifact demonstrating professional engineering rigor across user-space, kernel-space, hardware validation, and embedded Linux platform development.

```
