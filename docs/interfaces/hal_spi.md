# HAL SPI Interface

## Purpose

This document defines the contract boundary for SPI access on the BeagleBone Black side of VTCN. The SPI HAL exists to perform bounded full-duplex or half-duplex transfers with attached peripherals without absorbing protocol policy.

## Responsibilities

The SPI HAL is responsible for:

- opening and configuring the target SPI controller and chip-select path
- executing transfer requests with explicit buffer boundaries
- returning received bytes and transfer status
- surfacing controller or device-access errors to higher layers

## non-goals

The SPI HAL does not own:

- filtering
- calibration
- device protocol parsing beyond transfer mechanics
- telemetry framing
- daemon policy about polling cadence or publish behavior

HAL modules do not own filtering or calibration.

## Expected Inputs and Outputs

Inputs:

- bus and chip-select selection
- transfer configuration such as mode, clock, and transfer length
- transmit buffer contents when required

Outputs:

- received byte buffer
- transfer completion or failure status
- controller or device-access error information

## Error-Handling Direction

The SPI HAL should:

- fail clearly on open, configure, or transfer errors
- preserve explicit transfer boundaries
- avoid interpreting received bytes as higher-level telemetry inside the HAL
- leave retry and degradation policy to the daemon

## Phase 0 Limitation

This document does not claim validated SPI timing, mode compatibility, or target-device behavior on hardware.
