#include "vtcn/crank/RpmEstimator.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace {

// Walk the pulse sequence and average every adjacent interval except the one
// already identified as the missing-tooth gap.
//
// Contract:
// - `gap_index` names the later pulse in the long gap interval
// - the skipped interval is therefore `(pulses[gap_index - 1], pulses[gap_index])`
// - invalid or non-monotonic timestamps are rejected rather than interpreted
//   as a synthetic gap
[[nodiscard]] auto average_nominal_interval(std::span<const vtcn::crank::PulseEvent> pulses,
                                            std::size_t gap_index)
    -> std::optional<vtcn::crank::Interval>;

// Convert one nominal tooth interval into RPM using the supplied wheel profile.
//
// Phase 0 still calls this with the default 36-1 profile, but the helper takes
// the profile explicitly so the conversion rule is not hard-coded to `36`.
//
// Important:
// - RPM depends on tooth positions per revolution, not emitted pulse count
// - integer-only math keeps host-side behavior deterministic
[[nodiscard]] auto rpm_from_nominal_interval(
    vtcn::crank::Interval nominal_tooth_interval, const vtcn::crank::CrankWheelProfile &profile)
    -> std::optional<vtcn::crank::Rpm>;

auto average_nominal_interval(std::span<const vtcn::crank::PulseEvent> pulses,
                              const std::size_t gap_index)
    -> std::optional<vtcn::crank::Interval> {
    // Keep the running sum in chrono units so the intermediate state preserves
    // explicit "microseconds" meaning instead of becoming a raw integer early.
    vtcn::crank::Interval total_nominal_interval{0};
    std::size_t nominal_interval_count = 0U;

    // Each adjacent interval is formed from `pulses[i] - pulses[i - 1]`, so the
    // first complete interval appears at index 1.
    for (std::size_t i = 1; i < pulses.size(); ++i) {
        const auto current_timestamp = pulses[i].timestamp;
        const auto previous_timestamp = pulses[i - 1].timestamp;

        // Phase 0 requires strict monotonic increase so every interval is
        // positive and unambiguous.
        if (current_timestamp <= previous_timestamp) {
            return std::nullopt;
        }

        // The gap detector reports the later pulse index of the long interval.
        // Skip exactly that one interval when computing the nominal mean.
        if (i == gap_index) {
            continue;
        }

        total_nominal_interval += current_timestamp - previous_timestamp;
        ++nominal_interval_count;
    }

    // Estimation cannot continue if skipping the detected gap leaves no nominal
    // intervals to average.
    if (nominal_interval_count == 0U) {
        return std::nullopt;
    }

    // Integer chrono division keeps the result deterministic and aligned with
    // the integer microsecond timestamps produced by the host-side generator.
    const auto nominal_tooth_interval = total_nominal_interval / nominal_interval_count;

    if (nominal_tooth_interval.count() <= 0) {
        return std::nullopt;
    }

    return nominal_tooth_interval;
}

auto rpm_from_nominal_interval(vtcn::crank::Interval nominal_tooth_interval,
                               const vtcn::crank::CrankWheelProfile &profile)
    -> std::optional<vtcn::crank::Rpm> {
    // A non-positive interval is not physically meaningful for this estimator.
    if (nominal_tooth_interval.count() <= 0) {
        return std::nullopt;
    }

    // A wheel without tooth positions cannot define a revolution period.
    if (profile.tooth_positions_per_revolution == 0U) {
        return std::nullopt;
    }

    // Widen the arithmetic before multiplying so the intermediate math is
    // explicit and not dependent on the width of the chrono representation.
    const auto nominal_interval_us =
        static_cast<std::uint64_t>(nominal_tooth_interval.count());

    // Use tooth positions per revolution rather than emitted pulses. A 36-1
    // wheel still spans 36 tooth positions geometrically.
    const auto tooth_positions_per_revolution =
        static_cast<std::uint64_t>(profile.tooth_positions_per_revolution);
    const auto revolution_period_us = nominal_interval_us * tooth_positions_per_revolution;

    if (revolution_period_us == 0U) {
        return std::nullopt;
    }

    // Convert microseconds per revolution into revolutions per minute using
    // deterministic integer division.
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
    // Phase 0 is intentionally fixed to the repository's default 36-1 wheel.
    // The helper already accepts a profile so future profile-aware work can be
    // added without rewriting the conversion logic.
    constexpr auto kPhase0Profile = default_crank_wheel_profile();

    // Two pulses expose only one interval. Phase 0 requires at least one gap
    // interval plus at least one nominal interval to average.
    if (pulses.size() < 3) {
        return std::nullopt;
    }

    // `gap_index` names the later pulse in an adjacent interval, so valid
    // values are in the range [1, pulses.size() - 1].
    if (gap_result.gap_index == 0U || gap_result.gap_index >= pulses.size()) {
        return std::nullopt;
    }

    // First derive the nominal tooth interval from the observed timestamps.
    const auto nominal_tooth_interval = average_nominal_interval(pulses, gap_result.gap_index);
    if (!nominal_tooth_interval.has_value()) {
        return std::nullopt;
    }

    // Then convert that one-tooth interval into RPM using the Phase 0 profile.
    const auto estimated_rpm =
        rpm_from_nominal_interval(*nominal_tooth_interval, kPhase0Profile);
    if (!estimated_rpm.has_value()) {
        return std::nullopt;
    }

    // Return both the intermediate nominal interval and the final RPM so tests
    // can verify the estimator is not biased by the missing-tooth gap.
    return RpmEstimate{
        .estimated_rpm = *estimated_rpm,
        .nominal_tooth_interval = *nominal_tooth_interval,
    };
}

} // namespace vtcn::crank
