# ADR-000: Phase 0 Host-Side Crank Signal Pipeline Baseline

## Status

Accepted

## Context

VTCN needs a baseline decision record for the current host-side crank signal processing slice before deeper implementation work begins.

This baseline exists to capture the initial design direction for the weekend implementation and to make later changes explicit rather than accidental. It is intentionally written as a starting-point ADR that may be superseded or nullified as the project matures.

The immediate need is a host-side validation path for crankshaft pulse processing that:

- aligns with the existing HAL, daemon, and protocol boundaries
- does not overclaim hardware validation
- demonstrates modern C++ design and test discipline
- is narrow enough to complete during the current weekend sprint

## Decision

VTCN will implement a host-side crank signal processing slice with the following structure:

- `ICrankPulseSource` defines the acquisition boundary for timestamped crank pulse events
- `SimulatedCrankPulseSource` provides the initial host-side implementation
- daemon-side processing owns interval derivation, gap detection, and RPM estimation
- protocol framing remains separate from internal runtime structures and may remain optional in this slice

The initial modeled wheel profile is:

- 36 tooth positions per revolution
- 1 missing tooth

The simulation model will use:

- one timestamped pulse event per detected tooth passage
- tooth-event timing rather than full electrical waveform simulation
- deterministic monotonic timestamps
- integer `std::chrono::microseconds` rather than floating-point timestamps
- a longer inter-event interval to represent the missing-tooth gap

The current implementation direction also adopts the following C++ guidance for this slice:

- use value-oriented domain types
- use `std::chrono`, `std::span`, `std::optional`, and `constexpr` where they improve clarity and correctness
- use const-correctness and RAII as default engineering practice
- keep algorithmic processing components concrete unless a real substitution boundary exists
- use one interface at the acquisition boundary now, rather than introducing broad interface hierarchies across the whole pipeline
- prefer concrete types over speculative enums or type hierarchies unless a real second case exists

Code-facing names should describe domain responsibility rather than project phase. "Phase 0" remains documentation language, not the preferred naming convention for core runtime types.

## Consequences

- the host-side source is reusable for deterministic tests, demos, and offline validation
- the signal-processing logic can later be reused with real BBB or MCU-backed event sources
- the current slice stays honest about what is simulated versus what is hardware-validated
- the design remains extensible without requiring premature generalization of wheel profiles, protocol features, or transport paths
- the implementation stays interview-friendly by making the 36-1 timing model explicit and easy to explain
- future work can add additional source implementations behind `ICrankPulseSource` without rewriting the processing pipeline

## Deferred Work

The following are intentionally deferred for later phases unless they become necessary sooner:

- configurable crank wheel profiles beyond the default 36-1 model
- full waveform edge modeling including duty cycle, polarity, or sampled high/low durations
- richer result and error reporting with `std::expected`
- broader event or frame polymorphism with `std::variant`
- generic or policy-based algorithm variation using concepts or templates
- concurrency, coroutine-based pipelines, SIMD work, or allocator specialization
- modules, elaborate CRTP patterns, heavy metaprogramming, or expression-template numerics

These are deferred because the current Phase 0 goal favors clarity, deterministic behavior, and explainability over speculative abstraction.

## Nullification / Supersession Condition

This ADR is a baseline, not a permanent rule.

It should be considered nullified or superseded when one or more of the following happens:

- the host-side crank signal pipeline is replaced by a broader accepted ADR that covers both simulated and real acquisition paths
- the project introduces configurable wheel-profile support as a first-class requirement and the decision needs revision
- the acquisition boundary changes enough that `ICrankPulseSource` is no longer the correct seam
- later validated hardware integration justifies a different structure for event sourcing or runtime ownership

At that point, this ADR should either:

- be superseded by a new ADR, or
- be amended explicitly if the underlying decision remains the same and only the implementation details change

## Validation Boundary

This ADR records the intended baseline implementation direction for the Phase 0 crank signal pipeline. It is not proof of:

- BeagleBone Black hardware timing behavior
- real GPIO or ADC acquisition
- MCU timing accuracy
- real VR signal conditioning behavior
- target-side transport or framing behavior

Host-side simulation and tests validate structure, algorithm behavior, and repository discipline only.
