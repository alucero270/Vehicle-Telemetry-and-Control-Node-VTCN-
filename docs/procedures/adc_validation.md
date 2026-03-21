# ADC Validation

## Status

This is a future hardware-validation stub. No ADC validation has been performed yet.

## Purpose

Use this procedure later to validate BeagleBone Black ADC access for VTCN on real hardware.

## Intended Scope

When hardware is available, this procedure should capture:

- channel mapping used by the target wiring
- raw ADC read behavior through the selected Linux interface
- stability or noise observations under known input conditions
- any hardware-reported scale or reference information that affects safe interpretation
- captured evidence for later HAL and daemon work

## Blocked Until Hardware Arrives

The following remain blocked:

- target-side channel enumeration
- known-input measurement checks
- comparison against expected voltage or reference conditions
- repeated-read stability capture
- target-side evidence logs

## Planned Commands And Evidence

Placeholder only. Real command capture and outputs must be recorded when BBB hardware is available.

Expected evidence later:

- channel discovery output
- repeated sample capture
- notes about wiring and reference conditions
- log excerpts or saved data under `docs/test-results/`

## Validation Boundary

Host-only work must not be treated as ADC validation evidence. Only target-side execution of this procedure can validate BBB ADC behavior.
