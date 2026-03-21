# VTCN

Vehicle Telemetry and Control Node is a systems-oriented C++ project aimed at a disciplined telemetry node
architecture centered on the BeagleBone Black, with clear separation between hardware access, daemon-side
processing, and versioned telemetry interfaces.

## Current State

The repository is currently in Phase 0 Virtual Development Mode.

That means the active, reviewable slice is host-side only:
- CMake-based daemon scaffolding
- host-buildable C++ code and tests
- architecture and ADR documentation
- a weekend Phase 0 effort focused on crank pulse simulation, gap detection, and RPM estimation

That does not mean:
- BeagleBone hardware validation is complete
- HAL implementations are validated on target
- MCU timing behavior is proven
- transport, logging, or storage are production-ready

## Project Direction

The long-term project goal is to build a telemetry node that can:
- acquire signals from real hardware interfaces
- normalize and process those signals in a daemon layer
- publish versioned telemetry records to downstream consumers
- preserve evidence and validation discipline as the system matures

The intended architecture is:

```text
Physical Signals
    |
    v
HAL or MCU-facing transport
    |
    v
VTCN daemon
    |-- processing and estimation
    |-- framing
    |-- future logging, storage, and networking
    |
    v
Documented downstream consumers
```

## Architecture Principles

These principles are active now, not future decoration:
- HAL owns hardware I/O only
- daemon-side logic owns normalization, policy, processing, estimation, and framing
- protocol framing is separate from internal runtime structures
- internal C++ object layout is not a wire contract
- host-side validation is useful, but it is not hardware evidence

See:
- `docs/adr/ADR-001-hal-separation.md`
- `docs/adr/ADR-002-telemetry-framing.md`
- `docs/architecture/component_boundaries.md`
- `docs/procedures/virtual_development_mode.md`

## Phase 0 Focus

The current near-term implementation target is a host-side prototype inside `vtcn-daemon` that demonstrates:
- 36-1 crankshaft pulse generation with monotonic timestamps
- missing-tooth gap detection
- RPM estimation from pulse intervals
- clear CLI output and optionally simple telemetry framing
- deterministic unit and integration tests

This is intentionally a narrow slice. It is large enough to discuss systems thinking, signal processing,
boundary ownership, testing, and build discipline without pretending to be hardware validation.

## Repository Layout

```text
docs/
  adr/
  architecture/
  interfaces/
  procedures/

vtcn-daemon/
  include/
  src/
  tests/

toolchain/
mcu-node/
lkm/
systemd/
```

Interpretation:
- `docs/` holds the architecture truth layer
- `vtcn-daemon/` is the active host-side implementation area
- `toolchain/`, `mcu-node/`, `lkm/`, and `systemd/` remain future-facing until backed by code and evidence

## Build And Test

Current host-side commands:

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

These commands validate host-side build wiring and tests only.

## What Monday Should Show

For the immediate submission goal, the repo should demonstrate:
- modern C++ design in a small systems slice
- deterministic time-based signal handling
- clear module ownership
- real tests and clean build tooling
- documentation that is ambitious but honest

## Roadmap

Planned later phases may include:
- BeagleBone bring-up and target validation
- HAL implementations for BBB interfaces
- optional MCU-assisted deterministic capture
- telemetry framing expansion
- transport, storage, and deployment hardening

Those remain future work until backed by implementation and evidence.

## Archived Baseline

The previous top-level README was preserved at:
- `docs/readme-old.md`

It is kept as historical context, not as the current repo truth source.
