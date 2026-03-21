# VTCN Weekend Sprint Issue Pack

This pack is scoped to a Monday-submittable Phase 0 sample inside the existing VTCN repo.

Target outcome:
- a host-side only 36-1 crank pulse simulation
- gap detection
- RPM estimation
- a small CLI runtime
- real tests
- clean CMake and CI
- honest docs aligned to current ADRs

Non-goals for this sprint:
- BeagleBone hardware access
- MCU firmware work
- networking
- SQLite or storage
- protobuf
- daemon install or service work
- QEMU or kernel work

## Recommended Execution Order

1. Issue 01: audit repo scope and active docs
2. Issue 02: migrate active targets to C++23 and warnings
3. Issue 03: add formatting baseline
4. Issue 04: lock Phase 0 structure and core types
5. Issue 05: implement HostPulseSource
6. Issue 06: implement GapDetector
7. Issue 07: implement RpmEstimator
8. Issue 08: integrate Phase0Runtime CLI demo
9. Issue 09: add unit and integration tests
10. Issue 10: add GitHub Actions build and test workflow
11. Issue 11: README and design note cleanup
12. Issue 12: architecture conformance and Monday send audit
13. Issue 13: telemetry frame encoder and CRC, only if Issues 01-12 are complete

## Issue 01: Audit Repo Scope And Active Docs

Summary:
Trim or relabel speculative material so the repo reads as disciplined and current. The active repo story
must be "Phase 0 host-side validation slice inside a broader architecture" rather than "already validated
embedded product."

Acceptance criteria:
- README no longer claims all features require real hardware today
- active docs clearly distinguish current host-side validation from future hardware validation
- speculative or AI-heavy docs are archived, removed, or clearly labeled as non-active
- `docs/procedures/virtual_development_mode.md` is consistent with the current repo story

Dependencies:
- none

Estimated effort:
- 1.5 to 2.5 hours

Notes:
- This is first because credibility matters more than adding features to a misleading repo

## Issue 02: Migrate Active Targets To C++23 And Warning Baseline

Summary:
Move the active daemon slice to C++23 and enable a clean warning posture. This is a strong reviewer signal
and cheap to finish early.

Acceptance criteria:
- top-level and daemon CMake targets build as C++23
- warning flags are enabled for GCC, Clang, and MSVC where practical
- clean local build completes without new warnings in active targets
- test targets use the same standard level as production targets

Dependencies:
- Issue 01 recommended first so the repo narrative is already corrected

Estimated effort:
- 1 to 1.5 hours

## Issue 03: Add Formatting Baseline

Summary:
Add a formatter configuration and apply it to the active C++ slice. This is worth doing because the project
is still small and the diff cost is low.

Acceptance criteria:
- `.clang-format` exists at repo root
- active daemon headers, sources, and tests are formatted consistently
- formatting choices do not create churn in archived or inactive areas

Dependencies:
- Issue 02

Estimated effort:
- 0.5 to 1 hour

## Issue 04: Lock Phase 0 Structure And Core Types

Summary:
Define the exact file layout, type ownership, naming, and boundary rules for the Phase 0 slice before
filling in logic. This keeps the implementation discussable and prevents architecture drift.

Acceptance criteria:
- `vtcn-daemon` contains clear module placeholders for source, processing, runtime, and optional framing
- internal runtime types are separated from any future wire format
- a short design note documents data flow and ownership
- empty or stub implementations build after the structure change

Dependencies:
- Issue 02
- Issue 03 recommended

Estimated effort:
- 1.5 to 2 hours

## Issue 05: Implement HostPulseSource For 36-1 Simulation

Summary:
Implement a deterministic host-side source that emits monotonic pulse timestamps for a 36-1 wheel. The
missing tooth should be represented as a longer interval, not as a fake pulse.

Acceptance criteria:
- configurable RPM and revolution count
- 35 pulse timestamps per revolution
- timestamps are strictly increasing
- one oversized gap interval exists per revolution
- optional light jitter is either implemented deterministically or explicitly deferred

Dependencies:
- Issue 04

Estimated effort:
- 3 to 4 hours

## Issue 06: Implement GapDetector

Summary:
Derive tooth intervals from the ordered timestamps and detect the missing-tooth gap using a simple,
defensible algorithm.

Acceptance criteria:
- detects one gap per revolution on clean input
- remains stable under light jitter
- returns a structured result, not just a magic index
- behavior for insufficient input is explicit and testable

Dependencies:
- Issue 05

Estimated effort:
- 2 to 3 hours

## Issue 07: Implement RpmEstimator

Summary:
Estimate RPM from pulse intervals while excluding the missing-tooth gap from the nominal tooth interval.

Acceptance criteria:
- supports several RPM targets with documented tolerance
- handles malformed or insufficient input safely
- documents estimation assumptions
- implementation remains deterministic and testable

Dependencies:
- Issue 06

Estimated effort:
- 2 to 3 hours

## Issue 08: Integrate Phase0Runtime CLI Demo

Summary:
Wire the source, detector, and estimator into a small executable that demonstrates the full path and prints
clear, honest output for reviewers.

Acceptance criteria:
- CLI supports a small, documented argument set
- output shows target RPM, estimated RPM, detected gap information, and validation scope
- runtime remains host-side only and says so
- `--help` output is useful and current

Dependencies:
- Issue 07

Estimated effort:
- 2 to 3 hours

## Issue 09: Add Unit And Integration Tests

Summary:
Cover the signal source, gap detector, estimator, and end-to-end Phase 0 runtime with deterministic tests.

Acceptance criteria:
- unit tests cover pulse count, monotonicity, and gap shape
- unit tests cover gap detection on clean input and light jitter
- unit tests cover RPM estimation at several target RPM values
- integration test covers the full pipeline
- tests run locally through CTest without hardware

Dependencies:
- Issue 08, though some tests should be written alongside Issues 05-07

Estimated effort:
- 3 to 4 hours

## Issue 10: Add GitHub Actions Build And Test Workflow

Summary:
Add the smallest CI workflow that gives strong reviewer signal without becoming fragile.

Acceptance criteria:
- GitHub Actions workflow configures, builds, and runs tests
- Linux job exists at minimum
- workflow uses documented commands from README or TESTING.md
- status is suitable for linking in a Monday submission

Dependencies:
- Issue 09

Estimated effort:
- 1 to 2 hours

## Issue 11: README And Design Note Cleanup

Summary:
Rewrite the active repo story around the implemented Phase 0 slice, build commands, example usage, and
architecture honesty.

Acceptance criteria:
- README current-state section is accurate
- example commands are copy-pasteable
- sample output is included or reproducible
- a short design note explains signal generation, gap detection, and RPM estimation
- future work is clearly separated from current capability

Dependencies:
- Issue 08
- Issue 09
- Issue 10 recommended before final wording

Estimated effort:
- 2 to 3 hours

## Issue 12: Architecture Conformance And Monday Send Audit

Summary:
Run a final review pass focused on coherence, evidence discipline, and interview-readiness rather than new
features.

Acceptance criteria:
- active code and docs align with ADR-001 and ADR-002
- no file claims hardware validation that did not happen
- repo top-level story is consistent across README, docs, CLI help, and tests
- branch, PR, and submission plan are decided
- final sample is explainable in under five minutes

Dependencies:
- Issues 01 through 11

Estimated effort:
- 1.5 to 2 hours

## Issue 13: Optional Telemetry Frame Encoder And CRC

Summary:
If the core slice is already complete and stable, add a small versioned telemetry frame encoder plus CRC32.
This is optional because it can improve systems credibility, but it is not required for a strong Monday sample.

Acceptance criteria:
- internal runtime types remain separate from wire format types
- frame includes explicit version, timestamp, payload length, and CRC
- at least one encode test and one CRC test exist
- docs label framing as Phase 0 host-side only

Dependencies:
- Issue 08
- Issue 09
- Issue 11

Estimated effort:
- 2 to 4 hours

## Monday Minimum Deliverable

The minimum acceptable Monday sample is:
- honest repo scope and cleaned active docs
- C++23 daemon slice with warnings enabled
- HostPulseSource, GapDetector, and RpmEstimator implemented
- Phase0Runtime CLI demo
- unit and integration tests passing
- GitHub Actions build and test workflow
- README and design note updated

If time gets tight, drop Issue 13 first.

## Quality Bars

The sample should be:
- deterministic
- bounded in scope
- easy to build and run
- explicit about assumptions
- technically honest about validation
- easy to discuss in an interview

## Anti-Patterns To Avoid

- pretending host-side simulation proves hardware behavior
- adding interfaces before a real second implementation exists
- turning Phase 0 into a networking or storage project
- using floating-point timestamps for core event ordering
- hiding assumptions inside undocumented constants
- broad docs that describe future ambitions as current capability
