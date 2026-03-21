# Project Summary

VTCN (Vehicle Telemetry & Control Node) is an embedded Linux telemetry platform built around the BeagleBone Black, with an optional MCU coprocessor for deterministic acquisition tasks.

The node exists to:

- acquire real sensor data through BBB hardware interfaces and optional MCU transport
- process that data in a Linux daemon
- store useful local evidence
- export versioned telemetry to downstream consumers such as the Edge AI Diagnostics Platform

Core architecture:

- HAL handles hardware I/O only
- the daemon owns filtering, calibration, framing, logging, storage, and networking
- protocol documents define versioned external contracts
- the MCU is limited to deterministic acquisition support

Current development position:

- the project is in early repository and interface definition work
- Virtual Development Mode is active until target hardware is available
- host-side scaffolding and pure-logic tests are allowed
- hardware validation is deferred and must not be claimed from host-only work

Primary success condition:

- documented, testable, production-style embedded Linux engineering with real hardware evidence once hardware arrives
