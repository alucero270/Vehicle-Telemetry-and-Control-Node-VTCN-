# C++ Interview Prep Plan

This note is a focused study guide for discussing the active `vtcn-daemon`
code in a C++ interview.

Scope:
- `vtcn-daemon/`
- `vtcn-daemon/CMakeLists.txt`
- host-side runtime and tests
- modern C++ usage visible in the current Phase 0 slice

Build outputs and vendored test dependencies are intentionally excluded.

Important clarification:
- The active code targets modern C++ and uses several newer standard library
  facilities.
- The active code does not currently use C++ `concept` / `requires` syntax.
- If an interviewer says "concepts", be ready to distinguish formal language
  Concepts from general modern C++ ideas and STL usage.

## What This Project Demonstrates

This repository is a small systems-oriented C++23 slice that models:
- deterministic crank pulse generation
- missing-tooth gap detection
- RPM estimation from pulse timing
- separation between runtime orchestration and signal-processing logic
- unit and integration testing around a narrow but real pipeline

The easiest 60-second explanation is:

1. `SimulatedCrankPulseSource` generates deterministic pulse timestamps for a
   36-1 crank wheel.
2. `GapDetector` finds the missing-tooth gap by locating the longest adjacent
   pulse interval relative to the smallest interval.
3. `RpmEstimator` ignores that gap, averages the nominal tooth intervals, and
   computes RPM.
4. `CrankDemoRuntime` wires those pieces together and exposes a simple CLI.

## Areas You Need To Know

### High-priority C++ language and STL topics

- `std::chrono`
  - Used for `Timestamp` and `Interval`.
  - Know why strong time units are safer than raw integers.
  - Know how duration arithmetic and `.count()` work.

- `std::optional`
  - Used to model "valid result may be absent" in detection and estimation.
  - Know `has_value()`, dereference, `std::nullopt`, and when `optional` is
    better than exceptions.

- `std::span`
  - Used for non-owning access to pulse sequences.
  - Know why `std::span<const PulseEvent>` communicates "read-only view, no
    ownership transfer".

- `std::vector`
  - Used as the owning container for generated pulses.
  - Know contiguous storage, amortized growth, `reserve`, and value semantics.

- `std::string_view`
  - Used for CLI arguments and static app metadata.
  - Know why it is cheap, and know the lifetime hazards.

- Integer types and arithmetic
  - Know `std::size_t`, `std::uint32_t`, and widening to `std::uint64_t`.
  - Be ready to explain why the code prefers deterministic integer math over
    floating-point math for this Phase 0 slice.

- Object model basics
  - `virtual`, pure virtual functions, `override`, `final`, virtual
    destructors, and interface classes.
  - Explain why `ICrankPulseSource` has a virtual destructor.

- Function and type annotations
  - `const`
  - `noexcept`
  - `constexpr`
  - `[[nodiscard]]`

- Aggregate and designated initialization
  - The code frequently builds domain structs with designated initializers.
  - Know what aggregates are and why this style is readable here.

- Namespaces and translation-unit-local helpers
  - Know the difference between public API in `vtcn::...` and helper functions
    in anonymous namespaces inside `.cpp` files.

### Design topics you should be ready to explain

- Why small wrapper structs like `Rpm` and `Revolutions` are better than raw
  integers everywhere.
- Why the generator throws `std::invalid_argument` for invalid configuration,
  while the detector and estimator return `std::optional`.
- Why the runtime layer wires concrete objects directly today.
- Why `GapDetector` and `RpmEstimator` accept spans rather than owning
  containers.
- Why the code uses deterministic integer timing and fixed wheel assumptions in
  Phase 0.
- Why the interface seam exists even though the runtime currently constructs
  `SimulatedCrankPulseSource` directly.

### Testing and tooling topics

- GoogleTest basics: `TEST`, `EXPECT_*`, `ASSERT_*`
- deterministic tests
- boundary-condition testing
- invalid-input testing
- how `ctest` runs the test suite
- what `target_compile_features(... cxx_std_23)` means in CMake
- why warning flags are isolated in an interface target

## File Walkthrough Map

Use these files during prep:

- `vtcn-daemon/include/vtcn/crank/CrankTypes.hpp`
  - domain types, chrono aliases, `optional`, aggregate data structures

- `vtcn-daemon/include/vtcn/crank/ICrankPulseSource.hpp`
  - interface design, pure virtual methods, virtual destructor

- `vtcn-daemon/include/vtcn/crank/GapDetector.hpp`
  - `std::span` as a non-owning API boundary

- `vtcn-daemon/src/simulated_crank_pulse_source.cpp`
  - deterministic generation, exceptions, `vector::reserve`, arithmetic

- `vtcn-daemon/src/gap_detector.cpp`
  - `optional`, invariants, monotonic timestamp checks, threshold logic

- `vtcn-daemon/src/rpm_estimator.cpp`
  - helper functions in anonymous namespace, widening conversions, integer math,
    guarding against invalid input

- `vtcn-daemon/src/crank_demo_runtime.cpp`
  - orchestration, `string_view`, CLI handling, summary assembly

- `vtcn-daemon/tests/unit/*.cpp`
  - how behavior is validated and how interview claims can be backed by tests

## Five-Day Prep Schedule

### Day 1: Project story and core data modeling

Goals:
- Understand the end-to-end pipeline well enough to explain it without reading.
- Learn the domain types and the purpose of each major class.

Study:
- `vtcn-daemon/include/vtcn/crank/CrankTypes.hpp`
- `vtcn-daemon/src/crank_demo_runtime.cpp`
- `vtcn-daemon/src/main.cpp`

Focus topics:
- namespaces
- aggregate structs
- strong-ish domain typing with small wrapper structs
- `constexpr`, `noexcept`, `[[nodiscard]]`
- `std::string_view`

Practice:
- Explain what each struct represents.
- Explain the runtime flow from CLI entry to summary output.
- Explain why `string_view` is used for constant metadata and CLI argument
  handling.

End-of-day target:
- Give a 60-second walkthrough of the project.
- Give a 2-minute explanation of why the code is modern C++ instead of just
  "C with classes".

### Day 2: Containers, views, optional results, and chrono

Goals:
- Be comfortable with the standard library facilities most visible in the repo.

Study:
- `vtcn-daemon/include/vtcn/crank/GapDetector.hpp`
- `vtcn-daemon/src/gap_detector.cpp`
- `vtcn-daemon/src/rpm_estimator.cpp`

Focus topics:
- `std::span`
- `std::optional`
- `std::chrono::microseconds`
- `std::vector`
- integer arithmetic and `.count()`

Practice:
- Explain why `span` was chosen instead of `const std::vector<PulseEvent>&`.
- Explain why `optional` fits detector and estimator results.
- Explain why monotonic timestamps are required.
- Explain how the gap threshold is computed and what could go wrong with raw
  integer-based code if units were not explicit.

End-of-day target:
- Answer basic STL questions without hesitation.
- Explain the difference between owning and non-owning data structures.

### Day 3: Object model, interfaces, and error-handling design

Goals:
- Be ready for "why did you design it this way?" questions.

Study:
- `vtcn-daemon/include/vtcn/crank/ICrankPulseSource.hpp`
- `vtcn-daemon/include/vtcn/crank/SimulatedCrankPulseSource.hpp`
- `vtcn-daemon/src/simulated_crank_pulse_source.cpp`

Focus topics:
- pure virtual interfaces
- virtual destructors
- `override`
- `final`
- exceptions vs `optional`
- const-correctness

Practice:
- Explain why interface-based seams can be useful even if not heavily used yet.
- Explain why invalid profile / zero-RPM generation throws, but invalid pulse
  analysis returns `optional`.
- Explain why `generate(...)` returns a `std::vector<PulseEvent>` by value and
  why that is reasonable in modern C++.

End-of-day target:
- Give clean answers on abstraction, ownership, and failure modeling.

### Day 4: Testing, CMake, and engineering discipline

Goals:
- Be ready to talk about how you validate code, not just how you write it.

Study:
- `vtcn-daemon/CMakeLists.txt`
- `vtcn-daemon/tests/unit/crank_demo_runtime_test.cpp`
- `vtcn-daemon/tests/unit/gap_detector_test.cpp`
- `vtcn-daemon/tests/unit/rpm_estimator_tests.cpp`
- `vtcn-daemon/tests/unit/simulated_crank_pulse_source_test.cpp`

Focus topics:
- unit vs integration tests
- `EXPECT_*` vs `ASSERT_*`
- deterministic tests
- boundary testing
- CMake targets and test registration

Practice:
- Explain how the tests prove determinism.
- Explain what kinds of failures are covered.
- Explain the host-side validation story honestly.

End-of-day target:
- Be able to defend the quality of the repo with concrete test examples.

### Day 5: Mock interview day and answer refinement

Goals:
- Turn your understanding into concise, interview-ready answers.

Tasks:
- Rehearse the 60-second project explanation.
- Rehearse the 2-minute technical deep dive on the pulse pipeline.
- Rehearse answers to the mock Q&A below.
- Practice answering with file-backed examples instead of generic definitions.

Final review checklist:
- Can you explain `optional`, `span`, `string_view`, `chrono`, and `vector`
  using this repo?
- Can you explain ownership, lifetimes, and failure modes?
- Can you explain why this code is deterministic?
- Can you explain the role of tests and CMake?
- Can you clearly state that formal C++ Concepts are not used in the current
  code?

## Mock Interview Q&A

### 1. What does this project do?

It is a host-side C++23 prototype of a crank signal pipeline. It generates
deterministic pulse timestamps for a 36-1 wheel, detects the missing-tooth gap,
estimates RPM from the non-gap intervals, and exposes that through a small CLI
and test suite.

### 2. Why use `std::chrono::microseconds` instead of raw integers?

It makes time units explicit in the type system, which reduces unit-mismatch
bugs. It also keeps the arithmetic readable, since timestamps and intervals are
both duration-based values instead of anonymous integers.

### 3. Why use `std::optional` in the detector and estimator?

Those stages can fail for normal data-quality reasons such as too few pulses,
non-monotonic timestamps, or an invalid gap index. That is not necessarily an
exceptional program error, so `optional` is a good fit for "no valid result".

### 4. Why are exceptions still used in the generator?

The generator throws when the input configuration itself is invalid, such as a
zero RPM or an impossible wheel profile. That is different from "analysis could
not produce a valid estimate"; it is a caller/configuration error.

### 5. Why use `std::span<const PulseEvent>`?

The detector and estimator only need read-only access to an existing contiguous
pulse sequence. `span` expresses non-ownership clearly and keeps the API usable
with vectors and other contiguous storage without copying.

### 6. Why return `std::vector<PulseEvent>` by value?

The function is producing a new owning collection. Returning by value is
idiomatic modern C++, and move semantics make it efficient. It also keeps the
ownership model simple.

### 7. Why call `reserve` before pushing pulses?

The total pulse count is known up front, so reserving avoids repeated
reallocation and keeps the generator deterministic and efficient.

### 8. What is the purpose of wrapper structs like `Rpm` and `Revolutions`?

They give domain meaning to raw numbers and reduce accidental misuse. Passing a
plain integer is more error-prone because different quantities can be mixed up
more easily.

### 9. Why does `ICrankPulseSource` need a virtual destructor?

If an object is deleted through a base-class pointer or reference path, the
base class must have a virtual destructor so the derived destructor runs
correctly. It is part of making the interface safely polymorphic.

### 10. Why mark classes or structs as `final` here?

It communicates that these types are not intended to be base classes. It is a
clear design signal and can prevent accidental inheritance in a small system
where extension points are supposed to be explicit.

### 11. Why does the estimator widen to `std::uint64_t`?

It makes the intermediate multiplication explicit and reduces overflow risk
before computing RPM from microsecond intervals and tooth counts.

### 12. Why not use floating-point math for RPM?

For this Phase 0 slice, deterministic integer math is simpler and produces the
same output for the same input on every run. That is useful for tests and for a
signal-processing prototype where repeatability matters.

### 13. Why is `std::string_view` acceptable in the CLI code?

The code stores views to string literals and the process argument strings for
the lifetime of the call path. Those lifetimes are long enough for this use. It
would be dangerous if the code stored views into temporary strings.

### 14. What do the tests prove beyond simple correctness?

They prove determinism, boundary handling, and invalid-input handling. They
also show that the repo is not just architecture notes; the pipeline behavior
is executable and validated.

### 15. What modern C++ features are visible here?

`std::optional`, `std::span`, `std::string_view`, `std::chrono`, fixed-width
integer types, attributes like `[[nodiscard]]`, `constexpr`, `noexcept`,
aggregate initialization, interface-based polymorphism, and value-oriented
container usage.

### 16. Are C++ Concepts used here?

No. The code targets C++23 and uses modern standard library facilities, but it
does not currently define or use language Concepts via `concept` or `requires`.

## Fast Interview Checklist

Before the interview, make sure you can explain:
- the full pulse pipeline without reading
- why `chrono`, `optional`, `span`, `vector`, and `string_view` were used
- the difference between thrown exceptions and `optional`-based failure results
- the role of `ICrankPulseSource`
- how the tests validate determinism and boundary behavior
- what CMake targets exist and how tests are wired
- that the code uses modern C++23, but not formal C++ Concepts
