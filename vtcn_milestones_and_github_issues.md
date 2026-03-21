# VTCN Milestones And GitHub Issues

This document converts the weekend plan into GitHub-ready milestones, feature buckets, and issue cards.

The active submission target is Monday, March 23, 2026.

## Planning Intent

Use this file to:
- create GitHub milestones
- group issues under a few clear feature areas
- keep the repo focused on the Phase 0 host-side sample
- preserve the larger architecture plan without pulling future work into this weekend

## Recommended GitHub Labels

Core labels:
- `phase0`
- `submission`
- `architecture`
- `signal-processing`
- `tooling`
- `testing`
- `documentation`
- `ci`
- `optional`
- `future`

Priority labels:
- `p0`
- `p1`
- `p2`

Status labels if wanted:
- `blocked`
- `ready`
- `in-progress`
- `review`

## Milestones

### Milestone 1: Monday Submission - Phase 0 Host Signal Pipeline

Due date:
- March 23, 2026

Goal:
- deliver a clean, discussable, host-side prototype inside `vtcn-daemon`
- demonstrate 36-1 pulse simulation, gap detection, RPM estimation, tests, build tooling, and honest docs

Exit criteria:
- repo scope and README are technically honest
- active daemon targets build locally
- tests pass locally
- CI build-and-test workflow exists
- Phase 0 runtime demo is present
- code and docs align with current ADRs

Included work:
- all `p0` and `p1` issues in this document except explicitly marked optional

### Milestone 2: Post-Submission Hardening And Protocol Slice

Due date:
- no date yet

Goal:
- harden the host-side prototype after submission without expanding into hardware work

Exit criteria:
- optional telemetry framing is complete and tested
- warning baseline is stricter
- sample artifacts are cleaner
- docs reflect the more complete host-side slice

Included work:
- optional framing
- sanitizers if low friction
- extra integration coverage

### Milestone 3: Hardware Integration Prep

Due date:
- no date yet

Goal:
- prepare for BBB and MCU work without making unsupported claims

Exit criteria:
- hardware-facing interfaces are documented and narrowed
- bring-up procedures are realistic
- host-side assumptions are documented for target validation

Included work:
- HAL stubs and interface docs
- bring-up planning
- target evidence plan

## Feature Buckets

These are the best GitHub "feature" or epic-level buckets for the current repo.

### Feature A: Phase 0 Scope And Architecture Credibility

Why it exists:
- the repo already has architecture intent, but the active story needs to match current evidence

Included work:
- README correction
- active doc audit
- architecture conformance pass
- directory ownership decisions

### Feature B: Host-Side Crank Signal Pipeline

Why it exists:
- this is the actual technical centerpiece for Monday

Included work:
- HostPulseSource
- GapDetector
- RpmEstimator
- Phase0Runtime CLI

### Feature C: Quality Gates And Tooling

Why it exists:
- strong reviewer signal comes from build cleanliness, tests, and CI

Included work:
- C++23 migration
- warnings
- formatter
- unit tests
- integration tests
- GitHub Actions

### Feature D: Optional Telemetry Framing

Why it exists:
- framing is a credible systems addition, but only after the core signal pipeline is finished

Included work:
- frame encoder
- CRC32
- framing tests
- short design note

## Active Weekend Issues

Create these under Milestone 1 unless marked otherwise.

### Issue 01

Title:
- `Audit repo scope and active docs for Phase 0 honesty`

Summary:
- reconcile README, active docs, and virtual-development guidance so the repo presents a truthful Phase 0 story

Why it matters:
- this is a credibility issue, not cosmetic cleanup

Acceptance criteria:
- top-level README separates current state from long-term roadmap
- active docs do not claim host-side work proves hardware validation
- speculative or AI-heavy material is archived, relabeled, or removed from active paths
- `docs/procedures/virtual_development_mode.md` stays consistent with the repo story

Labels:
- `phase0`
- `submission`
- `documentation`
- `architecture`
- `p0`

Dependencies:
- none

Estimate:
- 2 hours

### Issue 02

Title:
- `Migrate active daemon targets to C++23`

Summary:
- upgrade active build targets from C++17 to C++23 and keep the standard consistent across library, CLI, and tests

Acceptance criteria:
- top-level CMake requests C++23
- `vtcn-daemon` targets inherit or require C++23
- local build succeeds after migration
- no unnecessary language feature churn is introduced just because C++23 is available

Labels:
- `phase0`
- `tooling`
- `p0`

Dependencies:
- Issue 01 recommended first

Estimate:
- 1 hour

### Issue 03

Title:
- `Add warning baseline for GCC Clang and MSVC`

Summary:
- enable useful compiler warnings now while the codebase is still small

Acceptance criteria:
- warnings are enabled in CMake for active targets
- build is clean under the current local compiler
- warning policy is documented briefly if needed

Labels:
- `phase0`
- `tooling`
- `p0`

Dependencies:
- Issue 02

Estimate:
- 1 hour

### Issue 04

Title:
- `Add clang-format baseline for active C++ files`

Summary:
- establish formatting early so the weekend does not produce noisy style churn

Acceptance criteria:
- `.clang-format` exists at repo root
- active `vtcn-daemon` code is formatted consistently
- inactive or archived content is not reformatted unnecessarily

Labels:
- `tooling`
- `submission`
- `p1`

Dependencies:
- Issue 02

Estimate:
- 45 minutes

### Issue 05

Title:
- `Define Phase 0 daemon structure and core runtime types`

Summary:
- create the exact file layout and runtime types for the host-side signal pipeline before filling in the logic

Acceptance criteria:
- `vtcn-daemon` has clear locations for source, processing, runtime, and optional protocol code
- internal runtime types are separate from future wire-format types
- stubs compile before feature logic is added
- ownership aligns with ADR-001 and ADR-002

Labels:
- `phase0`
- `architecture`
- `p0`

Dependencies:
- Issue 02
- Issue 03 recommended

Estimate:
- 2 hours

### Issue 06

Title:
- `Implement HostPulseSource for 36-1 crank pulse simulation`

Summary:
- generate deterministic monotonic pulse timestamps for a 36-1 wheel at configurable RPM and revolution count

Acceptance criteria:
- emits 35 pulses per revolution
- timestamps are strictly increasing
- one missing-tooth gap interval appears per revolution
- integer time units are explicit
- behavior is deterministic for identical inputs

Labels:
- `phase0`
- `signal-processing`
- `p0`

Dependencies:
- Issue 05

Estimate:
- 3 hours

### Issue 07

Title:
- `Implement gap detection for missing-tooth interval`

Summary:
- derive intervals from ordered pulses and identify the missing-tooth gap with a simple, defensible method

Acceptance criteria:
- works on clean data
- remains stable under light jitter
- insufficient input returns a clear failure result
- output includes enough information for runtime reporting and tests

Labels:
- `phase0`
- `signal-processing`
- `p0`

Dependencies:
- Issue 06

Estimate:
- 2.5 hours

### Issue 08

Title:
- `Implement RPM estimator from tooth intervals`

Summary:
- compute RPM from nominal tooth intervals while excluding the missing-tooth gap from the estimate

Acceptance criteria:
- estimates RPM within a documented tolerance for multiple target RPM values
- handles malformed and insufficient data safely
- algorithm assumptions are simple enough to explain in an interview

Labels:
- `phase0`
- `signal-processing`
- `p0`

Dependencies:
- Issue 07

Estimate:
- 2.5 hours

### Issue 09

Title:
- `Integrate Phase0Runtime CLI demo`

Summary:
- build a small CLI around the source, detector, and estimator so reviewers can run the full slice end to end

Acceptance criteria:
- CLI has a small documented argument set
- `--help` is current and honest
- output shows target RPM, estimated RPM, and gap detection summary
- runtime states clearly that it is host-side Phase 0 validation

Labels:
- `phase0`
- `submission`
- `p0`

Dependencies:
- Issue 08

Estimate:
- 2 hours

### Issue 10

Title:
- `Add unit tests for signal generation gap detection and RPM estimation`

Summary:
- build deterministic unit coverage around the core processing logic

Acceptance criteria:
- tests verify pulse count per revolution
- tests verify monotonic timestamps
- tests verify one oversized gap per revolution
- tests verify gap detection on clean input and light jitter
- tests verify RPM estimation tolerance at several RPM values

Labels:
- `phase0`
- `testing`
- `p0`

Dependencies:
- Issue 06
- Issue 07
- Issue 08

Estimate:
- 3 hours

### Issue 11

Title:
- `Add Phase 0 end-to-end integration tests`

Summary:
- verify the host-side pipeline works as a whole without relying on hardware

Acceptance criteria:
- integration test exercises source to estimator path
- CLI or runtime smoke test remains fast and deterministic
- tests are runnable through CTest

Labels:
- `phase0`
- `testing`
- `p1`

Dependencies:
- Issue 09
- Issue 10

Estimate:
- 1.5 hours

### Issue 12

Title:
- `Add GitHub Actions build-and-test workflow`

Summary:
- add the minimum worthwhile CI signal for an external reviewer

Acceptance criteria:
- Linux workflow configures, builds, and runs tests
- workflow uses documented commands
- badge or status reference is easy to share later if desired

Labels:
- `ci`
- `tooling`
- `submission`
- `p0`

Dependencies:
- Issue 10
- Issue 11 recommended

Estimate:
- 1.5 hours

### Issue 13

Title:
- `Rewrite README current-state section and add Phase 0 design note`

Summary:
- ensure reviewers can understand the implemented slice, build it, run it, and understand what has not yet been validated

Acceptance criteria:
- README has accurate current-state and roadmap sections
- build and test commands are correct
- sample run command is documented
- design note explains simulation, gap detection, and RPM estimation at a high level

Labels:
- `documentation`
- `submission`
- `phase0`
- `p0`

Dependencies:
- Issue 09
- Issue 10
- Issue 12 recommended

Estimate:
- 2 hours

### Issue 14

Title:
- `Run architecture conformance review before Monday submission`

Summary:
- do one last pass focused on boundary discipline, evidence discipline, and interview-readiness

Acceptance criteria:
- active code and docs align with ADR-001 and ADR-002
- no file overclaims validation status
- repo story is consistent across README, docs, code comments, and CLI output
- final branch and send strategy are chosen

Labels:
- `architecture`
- `submission`
- `p0`

Dependencies:
- Issues 01 through 13

Estimate:
- 1.5 hours

## Optional Weekend Issue

Only create this if Milestone 1 is already stable.

### Issue 15

Title:
- `Add optional telemetry frame encoder and CRC32 for Phase 0 output`

Summary:
- package runtime output into a small versioned frame without confusing internal runtime types with wire format types

Acceptance criteria:
- frame structure includes version, timestamp, payload length, and CRC32
- encoder is tested
- framing is documented as host-side only
- implementation does not disrupt the core pipeline schedule

Labels:
- `phase0`
- `optional`
- `signal-processing`
- `p2`

Milestone:
- Milestone 2 preferred unless finished early

Dependencies:
- Issue 09
- Issue 10
- Issue 13

Estimate:
- 3 hours

## Recommended Creation Order

Create milestones first:
1. Monday Submission - Phase 0 Host Signal Pipeline
2. Post-Submission Hardening And Protocol Slice
3. Hardware Integration Prep

Then create issues in this order:
1. Issue 01
2. Issue 02
3. Issue 03
4. Issue 05
5. Issue 06
6. Issue 07
7. Issue 08
8. Issue 09
9. Issue 10
10. Issue 11
11. Issue 12
12. Issue 13
13. Issue 14
14. Issue 04
15. Issue 15 only if time remains

Why Issue 04 is later in creation order:
- it is useful, but not on the critical path to a correct Monday sample

Why Issue 15 is separate:
- framing is a strong enhancement, but not stronger than finishing the core signal-processing slice cleanly

## Minimum Monday Deliverable

If time gets tight, the true minimum acceptable Monday package is:
- Issue 01
- Issue 02
- Issue 03
- Issue 05
- Issue 06
- Issue 07
- Issue 08
- Issue 10
- Issue 11
- Issue 12
- Issue 13
- Issue 14

The first thing to cut is:
- Issue 15

The second thing to relax, if absolutely necessary:
- full formatter rollout from Issue 04

Do not cut:
- README honesty
- tests
- buildability
- the end-to-end runtime demo
