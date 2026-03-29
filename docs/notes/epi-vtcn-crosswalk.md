# EPI To VTCN Crosswalk

This note ties `Elements of Programming Interviews in C++` to the active
`vtcn-daemon` code in this repository.

Use it when you want to answer questions like:

- "Where do these interview concepts show up in your project?"
- "Which EPI chapters are represented in your code?"
- "What is missing from the project that EPI still expects you to know?"

The goal is accuracy. This repo is a small systems-oriented C++ slice, not a
kitchen-sink algorithm repository.

## Best Matches In The Current Project

These are the EPI areas that map cleanly onto the current code.

### Chapter 5: Primitive Types

Where it shows up:

- `vtcn-daemon/include/vtcn/crank/CrankTypes.hpp`
- `vtcn-daemon/src/simulated_crank_pulse_source.cpp`
- `vtcn-daemon/src/rpm_estimator.cpp`
- `vtcn-daemon/tests/unit/simulated_crank_pulse_source_test.cpp`

What the project demonstrates:

- fixed-width integer usage with `std::uint32_t` and `std::uint64_t`
- integer arithmetic for deterministic time and RPM calculations
- explicit widening before multiplication
- careful domain modeling with wrapper structs like `Rpm` and `Revolutions`

Interview tie-in:

- EPI Chapter 5 is not just about bit hacks. It is also about disciplined
  integer reasoning.
- This project gives you a concrete example of avoiding floating-point drift by
  using integer microseconds and explicit units.

Good talking point:

- "We intentionally used integer microsecond timing so identical inputs produce
  identical timestamps and stable test output."

### Chapter 6: Arrays

Where it shows up:

- `vtcn-daemon/src/simulated_crank_pulse_source.cpp`
- `vtcn-daemon/src/gap_detector.cpp`
- `vtcn-daemon/src/rpm_estimator.cpp`
- `vtcn-daemon/tests/unit/gap_detector_test.cpp`
- `vtcn-daemon/tests/unit/rpm_estimator_tests.cpp`

What the project demonstrates:

- sequential scans over contiguous data
- index-based reasoning over adjacent elements
- one-pass min/max-style tracking in gap detection
- preallocation with `reserve`
- transforming raw pulse sequences into derived interval information

Interview tie-in:

- Many EPI array problems are really about scanning with invariants.
- `GapDetector` is a compact real-world example: walk once, maintain the
  smallest interval, maintain the largest interval, and validate monotonicity.

Good talking point:

- "The core detector is an array-scan problem over timestamped events, not a
  complex data structure problem."

### Chapter 7: Strings

Where it shows up:

- `vtcn-daemon/include/vtcn/runtime/CrankDemoRuntime.hpp`
- `vtcn-daemon/src/crank_demo_runtime.cpp`
- `vtcn-daemon/src/main.cpp`
- `vtcn-daemon/tests/unit/crank_demo_runtime_test.cpp`

What the project demonstrates:

- `std::string_view` for constant metadata and CLI arguments
- lightweight command dispatch
- output validation through string-based tests

Interview tie-in:

- The repo does not implement classic EPI string algorithms such as base
  conversion or substring search.
- It still gives you a clean C++ discussion point about string lifetimes and
  when `string_view` is appropriate.

Good talking point:

- "We used `string_view` where the storage lifetime was already owned by string
  literals or process arguments, so we got cheap non-owning access without
  copying."

### Chapter 12: Searching

Where it shows up:

- `vtcn-daemon/src/gap_detector.cpp`
- `vtcn-daemon/src/rpm_estimator.cpp`
- `vtcn-daemon/tests/unit/gap_detector_test.cpp`

What the project demonstrates:

- searching for a structural anomaly in ordered data
- validating ordering assumptions before using the data
- deriving a threshold from observed values

Interview tie-in:

- This is not binary search.
- It is still a search problem in the broader EPI sense: inspect data for the
  element or interval with the property you care about.

Good talking point:

- "The detector searches for the missing-tooth signature by finding the longest
  valid adjacent interval relative to the smallest observed interval."

### Chapter 18: Greedy Algorithms And Invariants

Where it shows up:

- `vtcn-daemon/src/gap_detector.cpp`
- `vtcn-daemon/src/rpm_estimator.cpp`
- `vtcn-daemon/tests/unit/gap_detector_test.cpp`

What the project demonstrates:

- maintaining running invariants during a single pass
- making a local decision based on min/max interval observations
- rejecting inputs that violate monotonicity or threshold assumptions

Interview tie-in:

- `GapDetector` is best explained as an invariant-driven scan.
- The important reasoning is not the syntax. It is why the detector can safely
  identify the gap from one pass over adjacent differences.

Good talking point:

- "The detector's invariant is that after scanning the first `i` pulses, we
  know the smallest and largest adjacent intervals seen so far and the index of
  the best gap candidate."

### Chapter 22: Language Questions

Where it shows up:

- `vtcn-daemon/include/vtcn/crank/CrankTypes.hpp`
- `vtcn-daemon/include/vtcn/crank/ICrankPulseSource.hpp`
- `vtcn-daemon/include/vtcn/crank/GapDetector.hpp`
- `vtcn-daemon/include/vtcn/crank/RpmEstimator.hpp`
- `vtcn-daemon/include/vtcn/runtime/CrankDemoRuntime.hpp`

What the project demonstrates:

- references vs values
- virtual interfaces and virtual destructors
- `override` and `final`
- `std::optional`
- `std::span`
- `std::vector`
- `std::string_view`
- `std::chrono`
- `constexpr`
- `noexcept`
- `[[nodiscard]]`

Interview tie-in:

- This is the strongest direct tie between EPI and the repo for a C++ role.
- The code gives you concrete examples for many language-level questions in
  Chapter 22.

Good talking point:

- "The project is small, but it demonstrates modern C++ value semantics,
  non-owning views, optional results, chrono-based units, and interface-based
  polymorphism."

### Chapter 23: Object-Oriented Design

Where it shows up:

- `vtcn-daemon/include/vtcn/crank/ICrankPulseSource.hpp`
- `vtcn-daemon/include/vtcn/crank/SimulatedCrankPulseSource.hpp`
- `vtcn-daemon/src/crank_demo_runtime.cpp`

What the project demonstrates:

- a narrow interface seam around pulse generation
- separation between generation, detection, estimation, and runtime wiring
- small domain types instead of passing raw integers everywhere

Interview tie-in:

- This is not a full OO design exercise like the later EPI system-design
  chapters.
- It is still a good example of pragmatic decomposition and controlled
  extension points.

Good talking point:

- "We separated the pipeline into a source, detector, estimator, and runtime
  orchestration layer so each piece stays testable and focused."

## Partial Matches

These chapters connect to the project, but only lightly.

### Chapter 9: Stacks And Queues

What exists:

- queue-like sequential processing of events
- command-line argument collection in a `vector`

What does not exist:

- explicit stack or queue algorithms
- parsing with a stack
- BFS queue workflows

Interview framing:

- Do not claim stack or queue algorithm depth from this repo.
- At most, say the project mainly uses linear pipelines rather than stack/queue
  data-structure problems.

### Chapter 14: Sorting

What exists:

- logic that assumes timestamps are already ordered

What does not exist:

- sorting as part of the algorithm
- interval merge logic
- order restoration from unsorted input

Interview framing:

- This repo consumes ordered data; it does not create order from disorder.

### Chapter 16: Recursion

What exists:

- decomposition into helper functions in `rpm_estimator.cpp`

What does not exist:

- recursive algorithms
- tree recursion
- backtracking

Interview framing:

- There is decomposition, but not recursive problem solving.

### Chapter 20: Parallel Computing

What exists:

- nothing in the active code path

What does not exist:

- threads
- locks
- atomics
- parallel scheduling

Interview framing:

- Be explicit that the repo is single-threaded Phase 0 host-side validation.

## Not Represented In The Current Project

These EPI chapters are essentially absent from the active code:

- Chapter 8: Linked Lists
- Chapter 10: Binary Trees
- Chapter 11: Heaps
- Chapter 13: Hash Tables
- Chapter 15: Binary Search Trees
- Chapter 17: Dynamic Programming
- Chapter 19: Graphs
- Chapter 21: Design Problems
- most classic Chapter 24 tooling topics
- Chapter 25: Honors Class problems

That is normal. A focused product slice should not try to demonstrate every
interview topic.

## Where Tests Strengthen The Story

EPI emphasizes not just solving problems, but validating them. The current repo
supports that well.

Relevant files:

- `vtcn-daemon/tests/unit/simulated_crank_pulse_source_test.cpp`
- `vtcn-daemon/tests/unit/gap_detector_test.cpp`
- `vtcn-daemon/tests/unit/rpm_estimator_tests.cpp`
- `vtcn-daemon/tests/unit/crank_demo_runtime_test.cpp`
- `vtcn-daemon/CMakeLists.txt`

What you can say:

- the generator is tested for deterministic output and boundary structure
- the detector is tested for correct gap identification, threshold behavior,
  and invalid input
- the estimator is tested for valid output, exclusion of the long gap, and
  error tolerance across representative RPM values
- the runtime is tested through CLI-style behavior and summary output

EPI tie-in:

- this is the "go from algorithm to tested program" part of interview prep
- it shows engineering discipline beyond whiteboard-only reasoning

## How To Talk About The Repo In EPI Terms

If an interviewer asks for EPI-style framing, use something like this:

"This project is strongest in the EPI areas around primitive types, arrays,
search-style scans, greedy invariants, and C++ language design. The core pulse
pipeline is a contiguous-sequence problem: generate deterministic timestamps,
scan for the anomalous long interval, and estimate RPM from the remaining
nominal intervals. It does not claim coverage of trees, graphs, heaps, or
dynamic programming, but it gives concrete examples of modern C++ interfaces,
ownership boundaries, optional results, chrono units, and test-backed
algorithmic reasoning."

## If You Want To Expand The Mapping Further

These are natural ways to add more EPI-aligned concepts to the repo:

- add a ring buffer or bounded queue for live telemetry ingestion
  - stronger Chapter 9 connection

- add a hash-based deduplication or cache layer for repeated telemetry frames
  - stronger Chapter 13 connection

- add a min/max heap for top-k anomaly windows or rolling diagnostics
  - stronger Chapter 11 connection

- add graph-like fault propagation or component dependency analysis
  - stronger Chapter 19 connection

- add a DP-style smoothing or sequence scoring experiment
  - stronger Chapter 17 connection

Those would be honest expansions, not forced interview gimmicks.
