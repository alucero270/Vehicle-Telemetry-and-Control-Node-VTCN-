# Debugging Workflow

## Status

This procedure is a staged debugging stub. It does not claim completed target-side debugging or hardware observations.

## Purpose

Use this procedure to define how VTCN debugging evidence should be gathered during host-only development now and during target debugging later.

## Current Host-Only Workflow

What can be done now:

- reproduce host build failures
- run host unit and integration tests
- inspect CMake and test output
- review logs emitted by host-side placeholder binaries
- refine documentation when architecture ambiguity is the root problem

## Future Target Workflow

When hardware is available, this procedure should later capture use of tools such as:

- `gdb`
- `strace`
- `perf`
- `dmesg`
- service logs and target process status

## Blocked Until Hardware Arrives

The following remain blocked:

- target-side interface debugging
- kernel and device-log capture from BBB during hardware faults
- target deployment troubleshooting
- timing-sensitive issue reproduction on real hardware

## Planned Evidence

Expected evidence to capture later:

- host or target command transcripts as applicable
- failure logs
- debugger backtraces when relevant
- `dmesg` excerpts for driver or kernel-facing faults
- stored artifacts under `docs/test-results/`

## Validation Boundary

A debugging note is only as strong as the environment it came from. Host-only debugging can validate host behavior and pure logic; it cannot be used as evidence for BBB hardware behavior until target-side runs are documented.
