# Testing (VTCN)

This project uses CMake, CTest, and GoogleTest for unit and integration testing.

## Goals
- Unit test all new or changed logic
- Keep tests deterministic and fast
- Prefer pure functions or narrowly scoped classes for signal processing logic
- Validate host-side behavior honestly without implying hardware validation

## Commands
- Configure:
  - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
- Build:
  - `cmake --build build`
- Run tests:
  - `ctest --test-dir build --output-on-failure`
- Optional verbose run:
  - `ctest --test-dir build -V`

If presets are added later, document those commands here.

## What Must Be Unit Tested

Minimum expectations for new logic:
- Signal generation behavior
  - correct pulse count for a 36-1 wheel
  - monotonic timestamp ordering
  - one oversized gap interval per revolution
- Gap detection behavior
  - detects the missing-tooth gap on clean input
  - remains stable under light jitter
- RPM estimation behavior
  - estimated RPM remains within a documented tolerance window
  - the gap interval is excluded or handled intentionally
  - malformed or insufficient data is handled safely
- Protocol framing and CRC behavior if framing is included in the current slice
- Argument or config parsing for runtime or demo behavior

## Integration Tests

Allowed and encouraged when they remain fast.

Examples:
- full pipeline: host pulse source -> gap detection -> RPM estimation
- Phase 0 runtime smoke test
- optional frame encode or decode round trip if protocol framing is implemented

Avoid:
- tests that require BeagleBone hardware
- tests that require MCU hardware
- tests that require real network services
- tests that depend on wall-clock timing
- slow, flaky, or nondeterministic tests

## Test Structure

- Unit tests live under `vtcn-daemon/tests/unit/`
- Integration tests live under `vtcn-daemon/tests/integration/`
- Prefer one feature area per test file:
  - `HostPulseSourceTests.cpp`
  - `GapDetectorTests.cpp`
  - `RpmEstimatorTests.cpp`
  - `FrameEncoderTests.cpp`
  - `Phase0RuntimeTests.cpp`

## Tolerance Guidance

- Prefer explicit tolerance windows over exact equality for derived numerical outputs
- Document why a tolerance is acceptable
- Keep tolerance tight enough to catch real regressions

## CI Expectations

At minimum, CI should run:
- configure
- build
- test

Nice to have if low risk:
- sanitizer job
- format or lint check
