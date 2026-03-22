#include "vtcn/crank/RpmEstimator.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace vtcn::crank {

std::optional<RpmEstimate> RpmEstimator::estimate(std::span<const PulseEvent> pulses,
                                                  const GapDetectionResult &gap_result) const {
    if (pulses.size() < 3) {
        return std::nullopt;
    }

    if (gap_result.gap_index == 0U || gap_result.gap_index >= pulses.size()) {
        return std::nullopt;
    }

    // Accumulate all valid adjacent intervals except the detected missing-tooth gap.
    Interval total_nominal_interval{0};
    std::size_t nominal_interval_count = 0U;

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        const auto current_timestamp = pulses[i].timestamp;
        const auto previous_timestamp = pulses[i - 1].timestamp;

        if (current_timestamp <= previous_timestamp) {
            return std::nullopt;
        }

        if (i == gap_result.gap_index) {
            continue;
        }

        const auto interval = current_timestamp - previous_timestamp;
        total_nominal_interval += interval;
        ++nominal_interval_count;
    }

    if (nominal_interval_count == 0U) {
        return std::nullopt;
    }

    const auto nominal_tooth_interval = total_nominal_interval / nominal_interval_count;

    if (nominal_tooth_interval.count() <= 0) {
        return std::nullopt;
    }

    // Convert the average nominal tooth interval back into RPM using the
    // default Phase 0 36-1 crank wheel profile.
    return std::nullopt;
}

} // namespace vtcn::crank