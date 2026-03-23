# VTCN (Vehicle Telemetry and Control Node)

VTCN is a systems-oriented C++ project focused on modeling and processing engine telemetry signals.

The current implementation is a **host-side prototype** that:

* Simulates a 36-1 crankshaft pulse signal
* Detects the missing-tooth gap
* Estimates RPM from pulse intervals

This project demonstrates systems thinking, signal processing, and clean C++ structure — not hardware validation.

---

## Quick Start

### Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Run CLI Demo

```bash
./build/vtcn-daemon/vtcn-daemon --demo
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

---

## What This Does

The current prototype models a common automotive crankshaft signal:

* Generates deterministic pulse timestamps for a 36-1 crank wheel
* Detects the missing-tooth gap (index position)
* Computes RPM based on pulse timing

All functionality runs on the host — no hardware required.

---

## Current State

This repository is in **Phase 0 – Virtual Development Mode**.

### Included

* CMake-based build system
* Host-side signal simulation
* Gap detection and RPM estimation
* Unit tests
* CLI demo

### Not Included (Yet)

* Real hardware integration
* BeagleBone Black validation
* HAL implementations on target
* Production telemetry transport or storage

---

## Architecture Overview

### Current (Phase 0)

```text
Simulated Signals
    |
    v
Gap Detection
    |
    v
RPM Estimation
    |
    v
CLI Output
```

### Target Architecture

```text
Physical Signals
    |
    v
HAL / MCU Layer
    |
    v
VTCN Daemon
    |
    v
Telemetry Output
```

---

## Project Goals

The long-term goal is to build a telemetry node that can:

* Acquire signals from real hardware
* Normalize and process those signals
* Produce structured telemetry output
* Maintain clear separation between hardware, processing, and interfaces

---

## Repository Layout

```text
docs/           # Architecture, ADRs, and procedures
vtcn-daemon/    # Active C++ implementation
toolchain/      # Build and environment tooling
roadmap/        # Future components and plans
```

---

## Design Principles

* Hardware access is isolated (HAL boundary)
* Processing logic is deterministic and testable
* Internal structures are not tied to wire formats
* Host-side validation is useful, but not hardware proof

See:

* `docs/adr/ADR-001-hal-separation.md`
* `docs/adr/ADR-002-telemetry-framing.md`

---

## Roadmap

Future phases may include:

* BeagleBone Black integration
* Real signal acquisition
* HAL implementation
* Telemetry transport and storage
* Hardware validation and testing

---

## Notes

This project is intentionally scoped as a **focused systems prototype**.

It is not meant to represent a complete embedded system, but rather a clean, reviewable slice of signal processing logic and architecture.
