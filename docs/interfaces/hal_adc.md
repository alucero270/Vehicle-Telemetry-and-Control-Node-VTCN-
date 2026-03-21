# HAL ADC Interface

## Purpose

This document defines the contract boundary for ADC access on the BeagleBone Black side of VTCN. The ADC HAL exists to expose hardware-facing analog input behavior to the daemon without absorbing signal-processing policy.

## Responsibilities

The ADC HAL is responsible for:

- opening and reading the target ADC or IIO interface exposed by Linux
- exposing channel-oriented raw sample access and device-read status
- reporting availability and read failures clearly to higher layers
- surfacing hardware-reported metadata when available and relevant to safe acquisition

## non-goals

The ADC HAL does not own:

- filtering
- calibration
- engineering-policy interpretation of sensor meaning
- telemetry framing
- storage or networking behavior

HAL modules do not own filtering or calibration.

## Expected Inputs and Outputs

Inputs:

- ADC channel selection
- read request or acquisition trigger from daemon-owned logic
- static device path or interface configuration

Outputs:

- raw ADC sample values
- device availability or read status
- hardware-reported metadata needed for safe interpretation, if available

## Error-Handling Direction

The ADC HAL should:

- surface missing device paths and read failures explicitly
- distinguish invalid or unavailable reads from successful reads
- avoid substituting fabricated values when the interface is unhealthy
- leave retry, filtering, and application-level handling policy to the daemon

## Phase 0 Limitation

This document does not claim that ADC channels, scaling behavior, or timing have been validated on BeagleBone Black hardware.
