# ADR-001: HAL Separation

## Status

Accepted

## Context

VTCN needs clear module boundaries so hardware access, daemon policy, and protocol logic do not collapse into one layer.

## Decision

HAL modules are limited to hardware I/O responsibilities such as:

- device access
- device status
- transport or read or write errors

HAL modules do not own:

- filtering
- calibration policy
- telemetry framing
- storage
- networking

## Consequences

- hardware-facing code remains easier to port and validate
- daemon logic remains the owner of signal policy and orchestration
- interface docs and future code reviews can reject policy drift into HAL

## Validation Boundary

This ADR records an architecture decision. It is not evidence that any HAL implementation has been validated on target hardware.
