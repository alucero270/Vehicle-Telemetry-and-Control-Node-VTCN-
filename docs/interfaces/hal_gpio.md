# HAL GPIO Interface

## Purpose

This document defines the contract boundary for GPIO access on the BeagleBone Black side of VTCN. The GPIO HAL exists to expose digital input, output, and edge-event access while keeping policy above the hardware boundary.

## Responsibilities

The GPIO HAL is responsible for:

- opening and configuring Linux GPIO resources through the chosen kernel-facing interface
- reading digital input state
- writing digital output state when that behavior is explicitly required
- surfacing edge or interrupt-style events when supported by the platform interface
- reporting line availability and access errors to higher layers

## non-goals

The GPIO HAL does not own:

- filtering
- calibration
- application-level debounce policy
- control-state decision making
- telemetry framing

HAL modules do not own filtering or calibration.

## Expected Inputs and Outputs

Inputs:

- line identifier or configured signal source
- read, write, or edge-subscription request
- line direction or edge-mode configuration supplied by daemon-owned setup logic

Outputs:

- digital line state
- edge or event notifications where supported
- line-status and access-failure information

## Error-Handling Direction

The GPIO HAL should:

- report configuration or access failure explicitly
- distinguish normal idle state from missing or failed line access
- avoid embedding debounce or business logic in the hardware layer
- leave policy decisions about degraded signals to the daemon

## Phase 0 Limitation

This document does not claim validated GPIO timing, interrupt behavior, or pin-mux correctness on target hardware.
