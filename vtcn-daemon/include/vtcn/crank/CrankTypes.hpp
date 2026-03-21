#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace vtcn::crank {

using Timestamp = std::chrono::microseconds;
using Interval = std::chrono::microseconds;

struct Revolutions final {
    std::size_t value{};
};

struct Rpm final {
    double value{};
};

struct CrankWheelProfile final {
    std::size_t tooth_positions_per_revolution{};
    std::size_t missing_tooth_count{};
};

struct CrankSignalConfig final {
    Rpm target_rpm{};
    Revolutions revolution_count{};
    Timestamp start_time{};
};

struct PulseEvent final {
    Timestamp timestamp{};
    std::size_t revolution_index{};
    std::size_t tooth_index{};
};

struct GapDetectionResult final {
    std::size_t gap_index{};
    Interval gap_interval{};
};

struct RpmEstimate final {
    Rpm estimated_rpm{};
    Interval nominal_tooth_interval{};
};

struct RuntimeSummary final {
    CrankWheelProfile profile{};
    Rpm target_rpm{};
    Revolutions revolution_count{};
    std::size_t pulses_generated{};
    std::optional<GapDetectionResult> gap_result{};
    std::optional<RpmEstimate> rpm_estimate{};
};

[[nodiscard]] constexpr CrankWheelProfile default_crank_wheel_profile() noexcept {
    return {.tooth_positions_per_revolution = 36U, .missing_tooth_count = 1U};
}

} // namespace vtcn::crank
