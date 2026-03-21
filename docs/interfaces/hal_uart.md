# HAL UART Interface

## Purpose

This document defines the contract boundary for UART access on the BeagleBone Black side of VTCN. The UART HAL exists to move raw serial bytes between Linux and attached devices while keeping message parsing and telemetry policy outside the HAL.

## Responsibilities

The UART HAL is responsible for:

- opening and configuring the target serial port
- reading and writing bounded byte streams
- exposing port availability, read status, and write status
- surfacing transport-level errors to higher layers

## non-goals

The UART HAL does not own:

- filtering
- calibration
- protocol parsing or checksum policy
- telemetry framing
- connection-health policy beyond reporting raw transport state

HAL modules do not own filtering or calibration.

## Expected Inputs and Outputs

Inputs:

- port selection and serial configuration
- bounded read request or outbound byte buffer
- timeout or blocking-mode choice supplied by higher-level logic

Outputs:

- raw byte buffers
- read or write status
- port error and availability information

## Error-Handling Direction

The UART HAL should:

- report port-open, read, write, and timeout failures explicitly
- keep raw byte transport separate from protocol interpretation
- avoid guessing frame validity inside the HAL
- leave retries, framing, and degraded-service policy to the daemon

## Phase 0 Limitation

This document does not claim validated UART timing, framing stability, or loopback behavior on target hardware.
