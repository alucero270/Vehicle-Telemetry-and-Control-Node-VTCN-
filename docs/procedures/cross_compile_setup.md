# Cross-Compile Setup

## Status

This procedure is a future-facing setup stub. It does not claim that the ARM toolchain, sysroot, deployment flow, or target execution path have been validated.

## Purpose

Use this procedure later to capture a reproducible host-to-BeagleBone cross-compilation workflow for VTCN.

## Intended Scope

When the toolchain work begins, this procedure should capture:

- selected ARM cross compiler and version
- host prerequisites
- sysroot or target-library strategy
- CMake toolchain usage
- build invocation
- deployment method
- proof that the produced binary runs on the BBB target

## Current Host-Only Position

What is allowed now:

- host-side CMake configuration
- host-side daemon builds
- host-side GoogleTest execution
- documentation of future toolchain expectations

What is not yet validated:

- BBB-targeted cross compilation
- target ABI compatibility
- sysroot correctness
- target deployment scripts
- execution of cross-compiled binaries on ARM hardware

## Blocked Until Hardware And Toolchain Work

The following items remain blocked or incomplete:

- final compiler selection for ARM deployment
- target sysroot capture
- linker and runtime dependency verification on BBB
- deployment and execution proof on target

## Planned Procedure Sections

### Prerequisites

Placeholder:

- selected compiler package
- host build tools
- target access path
- deployment transport

### Planned Commands

Placeholder only. Real commands should be recorded once the toolchain is selected and exercised.

### Planned Evidence

Expected evidence to capture later:

- toolchain version output
- successful cross-configure logs
- successful cross-build logs
- target-side execution proof

## Validation Boundary

Until this procedure is completed and target execution evidence is captured, host builds must not be presented as proof of ARM deployment success.
