# C++ Patterns Crosswalk

This note maps a prior Reddit discussion about practical C++ usage onto the
current `vtcn-daemon` codebase.

Scope:
- `vtcn-daemon/`
- `vtcn-daemon/CMakeLists.txt`
- host-side tests and runtime code only

Build outputs and vendored test dependencies are intentionally excluded.

## Present In The Code Today

### Unit testing

Unit testing is an active part of the repository, not a placeholder.

Relevant files:
- `vtcn-daemon/CMakeLists.txt`
- `vtcn-daemon/tests/unit/crank_demo_runtime_test.cpp`
- `vtcn-daemon/tests/unit/gap_detector_test.cpp`
- `vtcn-daemon/tests/unit/rpm_estimator_tests.cpp`
- `vtcn-daemon/tests/unit/simulated_crank_pulse_source_test.cpp`

Current usage:
- GoogleTest is located or fetched in CMake.
- Unit tests cover the CLI-facing runtime summary, pulse generation, gap
  detection, and RPM estimation.
- Tests exercise deterministic behavior and invalid-input handling.

### Runtime polymorphism through an interface

The codebase uses runtime polymorphism through `ICrankPulseSource`.

Relevant files:
- `vtcn-daemon/include/vtcn/crank/ICrankPulseSource.hpp`
- `vtcn-daemon/include/vtcn/crank/SimulatedCrankPulseSource.hpp`

Current usage:
- `ICrankPulseSource` defines a virtual `generate(...)` contract.
- `SimulatedCrankPulseSource` implements that contract.

Important nuance:
- The current runtime path does not consume the interface polymorphically.
- `crank_demo_runtime.cpp` constructs `SimulatedCrankPulseSource` directly,
  so the interface is a seam that exists, but is not yet fully exploited.

### Light dependency injection seams

There is some dependency injection, but it is narrow and pragmatic rather than
framework-driven.

Relevant files:
- `vtcn-daemon/include/vtcn/runtime/CrankDemoRuntime.hpp`
- `vtcn-daemon/src/main.cpp`
- `vtcn-daemon/tests/unit/crank_demo_runtime_test.cpp`
- `vtcn-daemon/tests/integration/crank_demo_smoke.cpp`

Current usage:
- `run_crank_demo_cli(...)` accepts output streams and argument vectors as
  parameters.
- `main.cpp` passes `std::cout` and `std::cerr`.
- Tests pass `std::ostringstream` objects, which makes CLI behavior testable
  without shelling out.

Important nuance:
- The crank pipeline itself is not injected.
- `crank_demo_runtime_summary()` still constructs the concrete pulse source,
  detector, and estimator internally.

### Standard library templates

The repo uses standard library templates heavily for containers, views, and
result types.

Relevant files:
- `vtcn-daemon/include/vtcn/crank/CrankTypes.hpp`
- `vtcn-daemon/include/vtcn/crank/GapDetector.hpp`
- `vtcn-daemon/include/vtcn/crank/RpmEstimator.hpp`

Current usage:
- `std::vector<PulseEvent>`
- `std::optional<GapDetectionResult>`
- `std::optional<RpmEstimate>`
- `std::span<const PulseEvent>`

Important nuance:
- This is normal modern C++ usage, but it is not the same thing as using
  custom templates for static polymorphism.

## Not Present In The Code Today

Repository search over `vtcn-daemon` found no current usage of:
- custom `template<...>` declarations
- `concept`
- `requires`
- `assert(...)`
- `static_assert(...)`
- `std::forward`
- Boost.SML or `sml::...`

Current error handling uses:
- `std::nullopt` for invalid or insufficient signal data
- `std::invalid_argument` for invalid crank profile or zero-RPM generation

That is a deliberate style difference from using `assert` as a primary guard.

## Where Stronger DI Would Fit Next

The most natural next step is not a DI framework. It is to push existing seams
one layer deeper.

Low-friction options:
- Change the demo pipeline so it accepts an `ICrankPulseSource` reference
  instead of always constructing `SimulatedCrankPulseSource`.
- Introduce a small pipeline object that receives its source, detector, and
  estimator in its constructor.
- Keep ownership simple. Prefer references or values over heap allocation
  unless lifetime actually requires indirection.

What that would buy:
- direct tests for alternate pulse sources
- easier fault-injection tests
- cleaner separation between wiring and algorithm logic

## Where Static Polymorphism Would Fit Next

Static polymorphism is not justified just because the code is in C++.

It would become reasonable if one of these becomes true:
- multiple detector strategies need to be swapped in a hot path
- wheel geometry becomes a compile-time policy rather than a runtime value
- the code needs compile-time guarantees around profile-specific behavior

Pragmatic examples:
- a templated detector policy for different wheel layouts
- a compile-time wheel profile type for fixed deployments
- `static_assert` checks once wheel configuration moves into compile-time data

Until then, the current concrete classes and one interface are the simpler
design.

## Where SML Or A State Machine Might Fit

Boost.SML is not a good fit for the current code because the demo is a linear
host-side pipeline:

1. generate pulses
2. detect gap
3. estimate RPM
4. print summary

It would become reasonable only if the daemon grows explicit lifecycle states,
for example:
- boot
- configure
- idle
- acquire
- degraded
- fault
- shutdown

For the current Phase 0 slice, introducing SML would add more structure than
the code needs.
