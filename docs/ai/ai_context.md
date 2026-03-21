# AI Context

## Load Order

When starting work, read the smallest relevant set of files in this order:

1. `README.md`
2. relevant files under `docs/architecture/`
3. relevant files under `docs/interfaces/`
4. relevant files under `docs/procedures/`
5. relevant ADRs and `docs/ai/*` as needed

## What To Assume

- the repository is the persistent project memory
- module boundaries in `docs/architecture/` are binding
- interface documents define intended contracts even before implementation exists
- documentation and tests are part of the deliverable, not follow-up work

## What Not To Assume

- shared chat memory across sessions
- undocumented APIs, payloads, or runtime behavior
- target hardware validation from host-only activity
- permission to collapse HAL, daemon, protocol, and MCU responsibilities

## Working Style

- make the smallest change that satisfies the current issue
- prefer conservative C++17 and clear module ownership
- add or update tests whenever code behavior changes
- suggest documentation updates whenever interfaces or procedures change
- stop and flag architecture ambiguity rather than inventing a design
