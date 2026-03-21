# Virtual Development Mode

## Status

This procedure defines the current operating mode for VTCN before target hardware arrives.

## Purpose

Virtual Development Mode allows disciplined progress on documentation, interfaces, build wiring, and host-testable logic without creating false hardware evidence.

## Current Active Slice

The active Phase 0 slice is host-side only.

It currently includes:

- repo and architecture governance
- host-buildable daemon scaffolding
- unit and integration test scaffolding
- a small CLI baseline in `vtcn-daemon`
- current weekend work toward 36-1 crank pulse simulation, gap detection, and RPM estimation

It does not currently include:

- validated BeagleBone hardware behavior
- validated HAL modules on target
- validated MCU timing or heartbeat behavior
- production-ready storage, networking, or deployment behavior

## Allowed Work

The following work is in scope during Virtual Development Mode:

- repository scaffolding
- architecture and interface documentation
- host-side CMake and GoogleTest scaffolding
- host-side signal simulation and pure processing logic
- protocol framing helpers and parser tests
- daemon skeleton work
- config, logging, storage, and networking scaffolds that do not require target hardware

## Not Allowed As Validation Claims

The following claims must not be made in Virtual Development Mode:

- BBB ADC success
- BBB GPIO interrupt success
- BBB I2C, SPI, or UART target success
- MCU timing verification
- target deployment success
- any claim that host-only output proves target behavior

## Standard Host-Side Commands

Current host-side validation commands:

```text
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

These commands validate host-only build and test scaffolding. They do not validate BeagleBone Black hardware behavior.

## Expected Evidence In This Mode

Acceptable evidence during this mode includes:

- successful host configure logs
- successful host build logs
- successful host test results
- reviewed architecture and interface documents

Unacceptable evidence includes fabricated or simulated hardware success claims.

## Blocked Work

The following work remains blocked pending hardware:

- BBB bring-up validation
- hardware interface validation procedures
- target-side timing measurement
- target deployment evidence
- any real-sensor acquisition claim

## Exit Condition

Virtual Development Mode ends only when target hardware is available and the relevant bring-up and validation procedures begin producing real evidence.
