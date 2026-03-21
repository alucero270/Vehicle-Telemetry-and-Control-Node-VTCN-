#include "vtcn/crank/RpmEstimator.hpp"

namespace vtcn::crank {

std::optional<RpmEstimate> RpmEstimator::estimate(std::span<const PulseEvent> pulses,
                                                  const GapDetectionResult &gap_result) const {
    (void)pulses;
    (void)gap_result;
    return std::nullopt;
}

} // namespace vtcn::crank
