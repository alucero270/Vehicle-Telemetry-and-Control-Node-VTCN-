# VTCN Governance Docs

Below are adapted versions of `REVIEWING.md`, `TESTING.md`, `CONTRIBUTING.md`, and `.github/pull_request_template.md` tailored for the VTCN C++ systems project.

---

# REVIEWING.md

```md
# Reviews (VTCN)

## Review Checklist

### Scope & Architecture
- Does this change only what the issue intended?
- Are module boundaries respected?
  - host-side pulse source / simulation
  - daemon processing
  - protocol framing
  - docs / build / CI
- Does the implementation align with current ADRs and Phase 0 constraints?
- Does the change avoid making unsupported hardware claims?
- Is the current slice still honest about Virtual Development Mode?

### Correctness
- Are timing units explicit and consistent?
  - prefer monotonic timestamps in integer microseconds or clearly documented equivalents
- Is 36-1 wheel behavior modeled correctly?
- Is the missing-tooth gap represented and detected correctly?
- Is RPM estimation derived from the correct interval set?
- Are tolerance thresholds documented and justified?
- Are boundary conditions handled?
  - empty pulse list
  - too few samples
  - malformed intervals
  - jitter/noise cases

### Modern C++ / Design Quality
- Does the code use modern C++23 features where they improve clarity or correctness?
- Are types strong enough to avoid primitive confusion?
- Is const-correctness respected?
- Are interfaces small and purposeful?
- Is logic testable, with pure functions where practical?
- Is the design simple enough for the current project stage, without unnecessary abstraction?

### Quality & Maintainability
- Warnings are enabled and the code builds cleanly
- Error handling is explicit and reasonable
- Naming is clear and consistent
- Logging/output is useful for debugging but not noisy
- Comments explain intent, not obvious syntax
- New code fits the repo structure and current architecture

### Testing & Tooling
- Unit tests added/updated for new logic
- Integration coverage exists for the end-to-end Phase 0 path where appropriate
- CI passes for build and tests
- New behavior includes at least one deterministic test
- Sanitizers or extra checks are used where practical and not destabilizing

### Docs & Evidence Discipline
- README/current-state docs were updated if behavior changed
- Design notes reflect what is actually implemented now
- Future work is clearly separated from current capability
- No AI-generated/speculative docs remain in active paths unless clearly labeled
- Reviewers can understand how to build, run, and test the current slice

## When to Request Changes

Request changes if:
- The PR exceeds the issue scope without a strong reason
- Architecture boundaries are blurred or violated
- Host-side work is written as if it proves hardware integration
- Tests were not added for new logic
- Timing assumptions or thresholds are unexplained
- Protocol/wire format changes happened without updating docs/ADR references
- CI/build tooling broke or is undocumented
- The repo now oversells implemented capability

## "Good Enough" Standard

This is a systems/embedded-adjacent C++ project.
Prioritize:
- correctness
- determinism at boundaries
- honest evidence
- clean modular design
- testability
- clarity over cleverness
```

---

# TESTING.md

```md
# Testing (VTCN)

This project uses CMake + CTest with GoogleTest for unit and integration testing.

## Goals
- Unit test all new or changed logic.
- Keep tests deterministic and fast.
- Prefer pure functions or narrowly scoped classes for signal processing logic.
- Validate host-side behavior honestly without implying hardware validation.

## Commands
- Configure:
  - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
- Build:
  - `cmake --build build`
- Run tests:
  - `ctest --test-dir build --output-on-failure`
- Optional verbose test run:
  - `ctest --test-dir build -V`

If presets are added later, document those commands here.

## What Must Be Unit Tested

Minimum expectations for new logic:
- Signal generation behavior:
  - correct pulse count for 36-1 wheel
  - monotonic timestamp ordering
  - one oversized gap interval per revolution
- Gap detection behavior:
  - detects missing-tooth gap on clean input
  - remains stable under light jitter/noise
- RPM estimation behavior:
  - estimated RPM remains within a documented tolerance window
  - gap interval is excluded or handled intentionally
  - malformed/insufficient data is handled safely
- Protocol framing / CRC behavior (if included in the current slice)
- Argument/config parsing for runtime/demo behavior

## Integration Tests
Allowed and encouraged when they remain fast.

Examples:
- Full pipeline: host pulse source -> gap detection -> RPM estimation
- Phase 0 runtime smoke test
- Optional frame encode/decode round trip if protocol framing is implemented

Avoid:
- Tests that require BeagleBone hardware
- Tests that require MCU hardware
- Tests that require real network services
- Tests that depend on wall-clock timing
- Slow, flaky, or nondeterministic tests

## Test Structure
- Unit tests live under:
  - `vtcn-daemon/tests/unit/`
- Integration tests live under:
  - `vtcn-daemon/tests/integration/`
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
- format/lint check
```

---

# CONTRIBUTING.md

```md
# Contributing to VTCN

VTCN follows disciplined change control to keep the project readable, maintainable,
and technically honest. The repo should reflect what is implemented now, what is host-side only,
and what is planned for later phases.

Codex or other AI tools may assist with implementation, but all changes must follow these conventions.

---

# Git Commit Guidelines

Each commit message consists of:
- Header (required)
- Body (strongly encouraged)
- Footer (optional)

No line may exceed 100 characters.

## Commit Message Format

`type(scope): subject`

Body (optional but strongly encouraged)
Footer (optional)

The header is mandatory.
The scope is required for this project.

## Revert
If reverting a commit:

`revert(scope): subject`

Body should include:
`This reverts commit <sha>.`

## Type
Must be one of:
- `feat`: a new feature
- `fix`: a bug fix
- `refactor`: code change without feature or bug fix
- `perf`: performance improvement
- `test`: add or update tests
- `docs`: documentation-only changes
- `style`: formatting only (no logic changes)
- `chore`: tooling, build, or dependency changes
- `ci`: CI/CD workflow changes

## Scope
Scope describes the subsystem affected.

Approved scopes for VTCN:
- `daemon` daemon runtime / orchestration
- `signal` pulse generation and timing logic
- `rpm` estimation logic
- `protocol` telemetry framing / encoding / decoding
- `crc` checksum logic
- `hal` hardware abstraction boundaries
- `mcu` MCU-side concepts or firmware-facing work
- `schema` schemas / payload definitions
- `toolchain` compiler / standard / build settings
- `build` CMake and build configuration
- `test` test code or test harness changes
- `docs` documentation updates
- `repo` repository structure changes
- `ci` CI workflows
- `systemd` service files / ops plumbing
- `phase0` virtual development mode implementation slice

Use lowercase.

## Examples
- `feat(signal): add 36-1 pulse generation`
- `feat(rpm): estimate rpm from tooth intervals`
- `test(phase0): add end-to-end runtime coverage`
- `chore(toolchain): migrate project to c++23`
- `ci(build): add cmake build-and-test workflow`

## Subject Rules
The subject must:
- use imperative present tense (`add`, not `added`)
- not capitalize the first letter
- not end with a period
- be concise and descriptive

Correct:
- `add monotonic pulse generation`
- `enforce c++23 across daemon targets`
- `archive speculative docs for later phases`

Incorrect:
- `Added pulse generation.`
- `Fixes stuff.`

## Body
Use imperative tense.
Explain:
- what changed
- why it changed
- how it differs from previous behavior

Keep lines under 100 characters.

Example:

Add a host-side 36-1 pulse source for Phase 0 validation.
Previously the repo documented the signal path but did not provide
an implemented, testable source of deterministic pulse events.
This adds timestamped pulses with optional light jitter for daemon-side processing.

Closes #12

## Footer
Used for:
- issue references: `Closes #12`, `Refs #14`
- breaking changes: must begin with `BREAKING CHANGE:`

---

# Branching Strategy

- One branch per issue when practical
- Branch name format:
  - `issue/<number>-short-description`

Examples:
- `issue/7-cpp23-migration`
- `issue/9-signal-generator`
- `issue/12-ci-build-test`

Rules:
- Merge via Pull Request
- Keep PRs scoped and reviewable
- Do not push directly to `main`
- Before external submission, ensure `main` reflects the sendable state or clearly document the review branch

---

# Code Style

## Language / Standard
- C++23 for active targets
- Prefer the standard library over ad hoc utilities where practical
- Use modern features when they improve correctness, readability, or maintainability

## Design Guidance
- Prefer clear value types and small classes
- Keep module responsibilities narrow
- Use pure functions where possible for core signal/rpm logic
- Avoid unnecessary inheritance or abstraction in Phase 0
- Separate internal runtime types from wire/protocol formats

## Naming
- Files: `PascalCase` or project-consistent naming, but be consistent
- Types/classes: `PascalCase`
- Functions/variables: `snake_case` or project-consistent lower style, but be consistent
- Constants: `kCamelCase` or `UPPER_SNAKE_CASE` if already standardized

## Error Handling
- Be explicit and predictable
- Avoid silent failure
- Prefer returning structured status/results where appropriate
- Do not use exceptions for ordinary control flow unless the project explicitly adopts that model

## Logging / Output
- Output should help a reviewer understand current runtime behavior
- Keep logs concise and deterministic
- Do not overclaim what the runtime proves

## Architecture Honesty
- Do not label host-side simulation as hardware validation
- Do not blur HAL responsibilities with daemon policy/processing
- Keep future-facing docs clearly separated from implemented behavior

---

# Security / Safety Basics

- No secrets in git
- No fabricated validation claims
- External interfaces and payload formats must be documented if added
- Changes to protocol framing should be reflected in docs and associated design notes

---

# Repo Discipline

Before opening a PR, ask:
- Does this reflect what the repo can actually do now?
- Can I explain this confidently in an interview?
- Did I add tests for the logic I changed?
- Did I keep the scope narrow enough to review quickly?
```

---

# .github/pull_request_template.md

```md
## What changed
-

## Why
-

## Architecture / ADR impact
- [ ] Boundary ownership preserved
- [ ] Virtual Development Mode assumptions still accurate
- [ ] Docs updated if implemented behavior changed

## How to test
- [ ] `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`
- [ ] `cmake --build build`
- [ ] `ctest --test-dir build --output-on-failure`
- [ ] Manual check:

## Reviewer notes
- Phase touched:
- Key tradeoffs:
- Known follow-up work:
```

