# Contributing To VTCN

VTCN follows disciplined change control so the repo stays readable, maintainable,
and technically honest. The repo should reflect what is implemented now, what is
host-side only, and what is planned for later phases.

AI tools may assist with implementation, but changes still need human review,
tests, and architecture discipline.

## Commit Message Guidelines

Each commit message consists of:
- header, required
- body, strongly encouraged
- footer, optional

No line may exceed 100 characters.

### Format

`type(scope): subject`

The header is mandatory. The scope is required for this project.

### Revert

If reverting a commit:

`revert(scope): subject`

The body should include:

`This reverts commit <sha>.`

### Types

Use one of:
- `feat`: a new feature
- `fix`: a bug fix
- `refactor`: code change without feature or bug fix
- `perf`: performance improvement
- `test`: add or update tests
- `docs`: documentation-only changes
- `style`: formatting only
- `chore`: tooling, build, or dependency changes
- `ci`: CI workflow changes

### Scopes

Approved scopes for VTCN:
- `daemon`
- `signal`
- `rpm`
- `protocol`
- `crc`
- `hal`
- `mcu`
- `schema`
- `toolchain`
- `build`
- `test`
- `docs`
- `repo`
- `ci`
- `systemd`
- `phase0`

Use lowercase.

### Examples

- `feat(signal): add 36-1 pulse generation`
- `feat(rpm): estimate rpm from tooth intervals`
- `test(phase0): add end-to-end runtime coverage`
- `chore(toolchain): migrate project to c++23`
- `ci(build): add cmake build-and-test workflow`

### Subject Rules

The subject must:
- use imperative present tense
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

### Body

Use the body to explain:
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

### Footer

Use the footer for:
- issue references, for example `Closes #12`
- breaking changes, beginning with `BREAKING CHANGE:`

## Branching Strategy

- Prefer one branch per issue when practical
- Branch name format:
  - `issue/<number>-short-description`

Examples:
- `issue/7-cpp23-migration`
- `issue/9-signal-generator`
- `issue/12-ci-build-test`

Rules:
- merge via pull request
- keep PRs scoped and reviewable
- do not push directly to `main`
- before external submission, either merge the sendable state to `main` or send a branch
  link with clear review guidance

## Code Style

### Language And Standard
- C++23 for active daemon targets
- prefer the standard library over ad hoc utilities where practical
- use modern features when they improve correctness, readability, or maintainability

### Design Guidance
- prefer clear value types and small classes
- keep module responsibilities narrow
- use pure functions where possible for core signal and RPM logic
- avoid unnecessary inheritance or abstraction in Phase 0
- separate internal runtime types from wire or protocol formats

### Naming
- files: project-consistent, prefer one convention and keep it consistent
- types and classes: `PascalCase`
- functions and variables: project-consistent lower style
- constants: `kCamelCase` or `UPPER_SNAKE_CASE` if standardized

### Error Handling
- be explicit and predictable
- avoid silent failure
- prefer structured status or result returns where appropriate
- do not use exceptions for ordinary control flow unless the project adopts that model

### Logging And Output
- output should help a reviewer understand runtime behavior
- keep logs concise and deterministic
- do not overclaim what the runtime proves

### Architecture Honesty
- do not label host-side simulation as hardware validation
- do not blur HAL responsibilities with daemon policy or processing
- keep future-facing docs clearly separated from implemented behavior

## Security And Safety Basics

- no secrets in git
- no fabricated validation claims
- external interfaces and payload formats must be documented if added
- changes to protocol framing should update docs and associated design notes

## Repo Discipline

Before opening a PR, ask:
- does this reflect what the repo can actually do now?
- can I explain this confidently in an interview?
- did I add tests for the logic I changed?
- did I keep the scope narrow enough to review quickly?
