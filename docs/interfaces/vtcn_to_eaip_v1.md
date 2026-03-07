# VTCN ↔ Edge AI Diagnostics Platform (EADP)

## Contract Goals

This document defines the **stable interface** between:

- **VTCN**: deterministic telemetry core running on an embedded Linux node (e.g., BeagleBone Black) alongside KittyBot
- **EADP**: AI diagnostics service that reads telemetry/logs, performs evidence-based troubleshooting, and produces human-facing artifacts

This contract is designed so **EADP can work with any client system** that implements this interface (KittyBot included, via VTCN).

Non-goals:
- Direct hardware control from EADP
- Unbounded data access
- Unverified AI claims

---

# System Responsibilities

## VTCN (Telemetry Core)
- Deterministic ingestion (MCU UART, GPIO/I2C/SPI/IIO as applicable)
- Framing + CRC validation + sequence checking
- Signal normalization + quality flags
- Local persistence (SQLite)
- Provides bounded read-only APIs for telemetry + logs
- Provides bounded endpoints for checklist execution **only if** checklists are implemented on the edge

## EADP (Diagnostics)
- Reads telemetry/logs via VTCN API
- Detects anomalies, correlates evidence
- Retrieves documentation (external knowledge service, e.g., Codex) and cites sources
- Produces human-facing outputs: checklists, incident reports
- Must degrade gracefully if VTCN or knowledge service is degraded

---

# Transport & Security

## Transport
- Default: **HTTP/1.1 REST + JSON**
- Optional: UDS (Unix domain socket) for same-host deployments

## Security
- Baseline: network isolated (robot LAN) + firewall rules
- Recommended: **mTLS** between EADP and VTCN when crossing a network boundary
- Optional: API key bearer token for additional guardrails

## Timeouts & Retries
- Client (EADP) timeout defaults:
  - Read endpoints: 1–2s
  - Log queries: 2–5s (bounded)
- Retries: max 1 retry with jitter for idempotent GETs

---

# Data Model

## Signal (normalized)
A single signal sample.

```json
{
  "ts_ms": 1700000123456,
  "name": "motor_l_rpm",
  "value": 123.4,
  "unit": "rpm",
  "quality": "good",
  "source": "mcu",
  "seq": 4242,
  "tags": {
    "subsystem": "drive",
    "platform": "kittybot",
    "version": "v1"
  }
}
```

### `quality` enum
- `good` (validated, current)
- `stale` (older than requested window)
- `invalid` (CRC/parse failure upstream)
- `missing` (expected but absent)

## Log Event
```json
{
  "ts_ms": 1700000123999,
  "level": "INFO",
  "component": "telemetryd",
  "event": "uart_frame_ok",
  "message": "Frame validated",
  "fields": {
    "seq": 4242,
    "bytes": 96,
    "crc": "0xA1B2C3D4"
  }
}
```

---

# API Endpoints (VTCN)

Base URL examples:
- Same-host: `http://localhost:8088`
- Network: `http://vtcn.local:8088`

All endpoints MUST return `application/json`.

## 1) Health

### `GET /health`
Returns service health and basic dependency status.

Response:
```json
{
  "status": "ok",
  "uptime_s": 12345,
  "version": "0.1.0",
  "deps": {
    "sqlite": "ok",
    "uart": "ok"
  },
  "time": {
    "now_ms": 1700000123456,
    "clock": "monotonic+realtime"
  }
}
```

## 2) Latest signals

### `GET /signals/latest?window_ms=2000&names=motor_l_rpm,motor_r_rpm&limit=200`
Returns latest samples within a time window.

Rules:
- `window_ms` caps lookback (server may clamp)
- `limit` is mandatory; server clamps to max (e.g., 500)
- Missing signals should be returned with `quality: missing` when `names` is provided

Response:
```json
{
  "window_ms": 2000,
  "signals": [ /* array of Signal */ ]
}
```

## 3) Signal history

### `GET /signals/range?from_ms=...&to_ms=...&name=motor_l_rpm&limit=500&cursor=...`
Returns paginated time-series samples for one signal.

Response:
```json
{
  "name": "motor_l_rpm",
  "from_ms": 1700000120000,
  "to_ms": 1700000125000,
  "items": [ /* Signal[] */ ],
  "next_cursor": "..."
}
```

## 4) Log query (bounded)

### `GET /logs/query?pattern=uart&level=INFO&from_ms=...&to_ms=...&limit=200&cursor=...`
Rules:
- `pattern` is optional; if omitted, returns recent logs
- Server clamps `to_ms-from_ms` to a maximum span (e.g., 10 minutes)
- Server clamps `limit`

Response:
```json
{
  "items": [ /* LogEvent[] */ ],
  "next_cursor": "..."
}
```

## 5) Metrics (optional, but recommended)

### `GET /metrics`
Either:
- Prometheus text format, or
- JSON metrics (`/metrics.json`)

Minimum metrics:
- ingestion rate
- parse failures
- crc failures
- dropped frames
- sqlite write latency

---

# Checklists (Optional Placement)

Preferred placement:
- **EADP owns checklist content**, because it is human workflow oriented.

However, VTCN may optionally expose execution tracking endpoints if you want the edge node to record runs.

## `GET /checklists`
Returns available checklists.

## `POST /checklists/{id}/runs`
Starts a run; returns run_id.

## `POST /checklists/{id}/runs/{run_id}/steps/{step_id}`
Marks a step complete, adds notes.

If VTCN does NOT implement checklists, omit these endpoints.

---

# Incident Reports

EADP generates incident reports. VTCN may store artifacts if desired.

## EADP incident report schema (portable)
```json
{
  "incident_id": "inc_2026_03_02_0001",
  "system": "kittybot",
  "summary": "Left motor RPM dropped to zero under load",
  "severity": "medium",
  "time_range": {"from_ms": 1700000120000, "to_ms": 1700000125000},
  "evidence": {
    "signals": [ /* Signal[] snapshots */ ],
    "logs": [ /* LogEvent[] excerpts */ ],
    "docs": [
      {
        "citation_id": "doc:procedures/drive/motor-troubleshooting.md#L120-L155",
        "title": "Motor troubleshooting",
        "excerpt": "..."
      }
    ]
  },
  "hypotheses": [
    {"id": "h1", "claim": "Encoder wiring intermittent", "support": ["signals", "docs"], "confidence": 0.62}
  ],
  "recommended_actions": [
    {"step": "Inspect encoder connector", "why": "RPM drops while PWM command steady"}
  ],
  "notes": "Generated by EADP; requires human verification"
}
```

---

# Error Handling

All non-2xx responses return:

```json
{
  "error": {
    "code": "bad_request",
    "message": "from_ms must be < to_ms",
    "details": {"field": "from_ms"}
  }
}
```

Recommended error codes:
- `bad_request`
- `not_found`
- `rate_limited`
- `dependency_down` (e.g., sqlite unavailable)
- `internal`

---

# Degraded Modes (Must Support)

## If VTCN is degraded
- `/health` returns `status: degraded` and flags dependency
- Read endpoints may return partial data with quality flags

## If knowledge service is down
- EADP must still:
  - read telemetry/logs
  - run heuristic checks
  - emit “degraded mode: docs unavailable”

---

# Versioning

- API version via path prefix **or** header.
- Recommended: `X-VTCN-API-Version: 1`
- Breaking changes require increment.

---

# Acceptance Criteria (v1)

- VTCN exposes `/health`, `/signals/latest`, `/signals/range`, `/logs/query`
- All endpoints are bounded (limits + pagination)
- Signal quality flags are implemented
- EADP can generate an incident report containing:
  - signal evidence
  - log evidence
  - (optional) document citations
- Degraded modes behave as specified

