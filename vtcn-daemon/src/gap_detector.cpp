#include "vtcn/crank/GapDetector.hpp"

namespace vtcn::crank {

std::optional<GapDetectionResult> GapDetector::detect(std::span<const PulseEvent> pulses) const {
    // The first valid adjacent interval seeds both the nominal and gap candidates.
    std::optional<Interval> nominal_interval{};
    std::optional<Interval> gap_interval{};
    std::size_t gap_index{};

    // Phase 0 requires at least two adjacent intervals before any gap decision is allowed.
    if (pulses.size() < 3) {
        return std::nullopt;
    }

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        const auto current_timestamp = pulses[i].timestamp;
        const auto previous_timestamp = pulses[i - 1].timestamp;

        // Invalid input must be rejected rather than interpreted as a synthetic gap.
        if (current_timestamp <= previous_timestamp) {
            return std::nullopt;
        }

        const auto interval = current_timestamp - previous_timestamp;

        if (!nominal_interval.has_value()) {
            // intervals[i - 1] is the gap between pulses[i - 1] and pulses[i],
            // so the later pulse index is the contract's gap_index.
            nominal_interval = interval;
            gap_interval = interval;
            gap_index = i;
        } else {
            if (interval < *nominal_interval) {
                nominal_interval = interval;
            }

            if (interval > *gap_interval) {
                gap_interval = interval;
                gap_index = i;
            }
        }
    }

    // Integer-only threshold: detect a gap only when the largest interval is
    // strictly greater than 1.5x the smallest observed interval.
    const auto threshold = *nominal_interval + *nominal_interval / 2;

    if (*gap_interval <= threshold) {
        return std::nullopt;
    }

    return GapDetectionResult{
        .gap_index = gap_index,
        .gap_interval = *gap_interval,
    };
}

} // namespace vtcn::crank
