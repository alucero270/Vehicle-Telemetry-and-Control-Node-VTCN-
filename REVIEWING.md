# Reviews (VTCN)

## Review Checklist

### Scope And Architecture
- Does this change stay inside the issue scope?
- Are module boundaries respected?
  - host-side pulse source or simulation
  - daemon processing
  - protocol framing
  - docs, build, or CI
- Does the implementation align with current ADRs and Phase 0 constraints?
- Does the change avoid making unsupported hardware claims?
- Is Virtual Development Mode still described honestly?

### Correctness
- Are timing units explicit and consistent?
  - prefer monotonic timestamps in integer microseconds or clearly documented equivalents
- Is 36-1 wheel behavior modeled correctly?
- Is the missing-tooth gap represented and detected correctly?
- Is RPM estimation derived from the correct interval set?
- Are tolerance thresholds documented and justified?
- Are edge cases handled?
  - empty pulse list
  - too few samples
  - malformed intervals
  - jitter or noise cases

### Modern C++ And Design Quality
- Does the code use modern C++23 where it improves clarity or correctness?
- Are types strong enough to avoid primitive confusion?
- Is const-correctness respected?
- Are interfaces small and purposeful?
- Is logic testable, with pure functions where practical?
- Is the design simple enough for Phase 0, without speculative abstraction?

### Maintainability
- Warnings are enabled and the code builds cleanly
- Error handling is explicit and predictable
- Naming is clear and consistent
- Output is useful for debugging but not noisy
- Comments explain intent, not syntax
- New code fits the repo structure and current architecture

### Testing And Tooling
- Unit tests were added or updated for new logic
- Integration coverage exists for the end-to-end Phase 0 path where appropriate
- CI passes for build and tests
- New behavior includes at least one deterministic test
- Sanitizers or extra checks are used where practical and low risk

### Docs And Evidence Discipline
- README or current-state docs were updated if behavior changed
- Design notes reflect what is actually implemented now
- Future work is clearly separated from current capability
- Speculative or AI-heavy docs are archived, relabeled, or removed from active paths
- A reviewer can understand how to build, run, and test the current slice

## When To Request Changes

Request changes if:
- the PR exceeds the issue scope without a strong reason
- architecture boundaries are blurred or violated
- host-side work is written as if it proves hardware integration
- tests were not added for new logic
- timing assumptions or thresholds are unexplained
- protocol or wire format changes happened without corresponding docs
- CI or build tooling broke or became harder to run
- the repo now oversells implemented capability

## Good Enough Standard

This is a systems-oriented C++ project. Prioritize:
- correctness
- deterministic behavior at boundaries
- honest evidence
- clean modular design
- testability
- clarity over cleverness
