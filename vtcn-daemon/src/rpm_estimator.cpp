#include "vtcn/crank/RpmEstimator.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace {

// Average adjacent intervals while excluding the detected missing-tooth gap.
[[nodiscard]] auto average_nominal_interval(std::span<const vtcn::crank::PulseEvent> pulses,
                                            std::size_t gap_index)
    -> std::optional<vtcn::crank::Interval>;

// Convert a nominal tooth interval into RPM using the wheel geometry.
[[nodiscard]] auto rpm_from_nominal_interval(
    vtcn::crank::Interval nominal_tooth_interval, const vtcn::crank::CrankWheelProfile &profile)
    -> std::optional<vtcn::crank::Rpm>;

auto average_nominal_interval(std::span<const vtcn::crank::PulseEvent> pulses,
                              const std::size_t gap_index)
    -> std::optional<vtcn::crank::Interval> {
    vtcn::crank::Interval total_nominal_interval{0};
    std::size_t nominal_interval_count = 0U;

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        const auto current_timestamp = pulses[i].timestamp;
        const auto previous_timestamp = pulses[i - 1].timestamp;

        if (current_timestamp <= previous_timestamp) {
            return std::nullopt;
        }

        // `gap_index` names the later pulse in the long interval.
        if (i == gap_index) {
            continue;
        }

        total_nominal_interval += current_timestamp - previous_timestamp;
        ++nominal_interval_count;
    }

    if (nominal_interval_count == 0U) {
        return std::nullopt;
    }

    const auto nominal_tooth_interval = total_nominal_interval / nominal_interval_count;

    if (nominal_tooth_interval.count() <= 0) {
        return std::nullopt;
    }

    return nominal_tooth_interval;
}

auto rpm_from_nominal_interval(vtcn::crank::Interval nominal_tooth_interval,
                               const vtcn::crank::CrankWheelProfile &profile)
    -> std::optional<vtcn::crank::Rpm> {
    if (nominal_tooth_interval.count() <= 0) {
        return std::nullopt;
    }

    if (profile.tooth_positions_per_revolution == 0U) {
        return std::nullopt;
    }

    // Widen before multiplying to keep the intermediate arithmetic explicit.
    const auto nominal_interval_us =
        static_cast<std::uint64_t>(nominal_tooth_interval.count());

    // A 36-1 wheel still spans 36 tooth positions per revolution.
    const auto tooth_positions_per_revolution =
        static_cast<std::uint64_t>(profile.tooth_positions_per_revolution);
    const auto revolution_period_us = nominal_interval_us * tooth_positions_per_revolution;

    if (revolution_period_us == 0U) {
        return std::nullopt;
    }

    const auto estimated_rpm =
        static_cast<std::uint32_t>(60'000'000ULL / revolution_period_us);

    if (estimated_rpm == 0U) {
        return std::nullopt;
    }

    return vtcn::crank::Rpm{estimated_rpm};
}

} // namespace

namespace vtcn::crank {

std::optional<RpmEstimate> RpmEstimator::estimate(std::span<const PulseEvent> pulses,
                                                  const GapDetectionResult &gap_result) const {
    // Phase 0 is fixed to the repository's default 36-1 wheel.
    constexpr auto kPhase0Profile = default_crank_wheel_profile();

    if (pulses.size() < 3) {
        return std::nullopt;
    }

    // Valid `gap_index` values are [1, pulses.size() - 1].
    if (gap_result.gap_index == 0U || gap_result.gap_index >= pulses.size()) {
        return std::nullopt;
    }

    const auto nominal_tooth_interval = average_nominal_interval(pulses, gap_result.gap_index);
    if (!nominal_tooth_interval.has_value()) {
        return std::nullopt;
    }

    const auto estimated_rpm =
        rpm_from_nominal_interval(*nominal_tooth_interval, kPhase0Profile);
    if (!estimated_rpm.has_value()) {
        return std::nullopt;
    }

    return RpmEstimate{
        .estimated_rpm = *estimated_rpm,
        .nominal_tooth_interval = *nominal_tooth_interval,
    };
}

} // namespace vtcn::crank
