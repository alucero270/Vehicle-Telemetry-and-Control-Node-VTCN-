# HAL I2C Interface

## Purpose

This document defines the contract boundary for I2C access on the BeagleBone Black side of VTCN. The I2C HAL exists to perform bus transactions with addressed peripherals while keeping device-specific interpretation above the HAL layer.

## Responsibilities

The I2C HAL is responsible for:

- opening and configuring the target I2C bus
- executing bounded read, write, or combined transactions to an addressed device
- returning raw bytes and transaction status
- surfacing bus-level errors and device-access failures clearly

## non-goals

The I2C HAL does not own:

- filtering
- calibration
- device-specific register interpretation policy
- telemetry framing
- storage, retry policy orchestration, or network behavior

HAL modules do not own filtering or calibration.

## Expected Inputs and Outputs

Inputs:

- bus selection
- target device address
- transaction description such as read length, write payload, or register-oriented exchange

Outputs:

- raw bytes returned by the device
- transaction success or failure status
- bus-access error information

## Error-Handling Direction

The I2C HAL should:

- surface NACK, timeout, and bus-access failures explicitly
- avoid decoding device protocol semantics inside the HAL
- return enough status for the daemon to decide retry or degradation behavior
- avoid converting transaction failure into fabricated sensor data

## Phase 0 Limitation

This document does not claim validated bus timing, pull-up behavior, or peripheral interoperability on real hardware.
